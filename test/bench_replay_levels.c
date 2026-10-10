#define _POSIX_C_SOURCE 200809L
#include "replay_fixture.h"
#include "../src/vsdlss_parallel.h"
#include <time.h>
#include <stdio.h>
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+1e-9*t.tv_nsec;}
static int cmpd(const void*a,const void*b){double x=*(const double*)a,y=*(const double*)b;return(x>y)-(x<y);}
int main(int argc,char **argv)
{
    csi width=argc>1?atoll(argv[1]):48000,depth=argc>2?atoll(argv[2]):80;
    int overlap=argc>3?atoi(argv[3]):0,reps=argc>4?atoi(argv[4]):7,pack=argc>5?atoi(argv[5]):1;
    int max_threads=argc>6?atoi(argv[6]):16;
    if(width<2||depth<1||width>1000000||depth>10000||width*(depth+1)>0x3fffffff||reps<1||reps>31||overlap<0||overlap>2||(overlap&&(width&1))||(pack!=0&&pack!=1)||max_threads<1||max_threads>1024)return 2;
    vsdlss_reduction *r=replay_fixture(width,depth,1,overlap),*baseline=replay_fixture(width,depth,1,overlap);if(!r||!baseline)return 2;
    size_t bytes=(size_t)r->n*8;double *b=malloc(bytes),*fw=malloc(bytes),*ref=malloc(bytes),*w=malloc(bytes);
    if(!b||!fw||!ref||!w)return 2;
    for(csi i=0;i<r->n;i++)b[i]=0.3+sin(i*0.13);
    setenv("VSDLSS_REPLAY_LEVELS","0",1);vsdlss_set_num_threads(1);
    memcpy(fw,b,bytes);if(vsdlss_reduce_forward_inplace(r,fw,NULL))return 3;
    memcpy(ref,fw,bytes);if(vsdlss_reduce_backward_inplace(r,NULL,ref))return 3;
    setenv("VSDLSS_REPLAY_LEVELS","1",1);double start=now();
    if(vsdlss_reduce_build_levels(r)!=VSDLSS_OK)return 3;
    if(!r->pk[0].levels){fprintf(stderr,"no parallel schedule retained\n");return 3;}
    setenv("VSDLSS_REPLAY_PACK",pack?"1":"0",1);
    if(pack && vsdlss_reduce_pack_levels(r)!=VSDLSS_OK)return 3;
    /* As production relabel does: use degree bytes when vertex order is
     * contiguous (the unshared layer-major fixture already has this order). */
    if(pack && !overlap) {
        vsdlss_pk_seg *g=r->pk;g->deg=malloc((size_t)g->count);if(!g->deg)return 2;
        for(csi i=0;i<g->count;i++){if((g->head[i]&0x3fffffff)!=(uint32_t)i)return 3;g->deg[i]=(uint8_t)(g->head[i]>>30);}
        free(g->head);g->head=NULL;
    }
    double build=now()-start;vsdlss_replay_levels *s=r->pk[0].levels;
    if(!s){fprintf(stderr,"no parallel schedule retained\n");return 3;}
    printf("# synthetic only n=%lld records=%lld width=%lld depth=%lld shared_targets=%d levels=%lld schedule_ms=%.6f schedule_MB=%.3f pack=%d\n",
           (long long)r->n,(long long)r->count,(long long)width,(long long)depth,overlap,(long long)s->levels,1000*build,
           (double)((s->ref?s->count*sizeof(*s->ref):0)+(s->levels+1)*sizeof(csi)+s->tiles*sizeof(*s->tile))/1e6,pack);
    puts("threads,enabled,direction,median_ms,min_ms,max_ms,bitwise_equal,team_used");fflush(stdout);
    for(int nt=1;nt<=max_threads;nt*=2){
        if(nt>1&&!vsdlss_parallel_enabled())break;
        vsdlss_set_num_threads(nt);
        for(int dir=0;dir<2;dir++){
            double t[2][31];int equal[2]={1,1},teams[2]={1,1};
            for(int k=-2;k<reps;k++)for(int turn=0;turn<2;turn++){
                int mode=turn^((k+2)&1);setenv("VSDLSS_REPLAY_LEVELS",mode?"1":"0",1);
                vsdlss_set_num_threads(nt);
                memcpy(w,dir?fw:b,bytes);start=now();
                const vsdlss_reduction *use=mode?r:baseline;
                vsdlss_status st=dir?vsdlss_reduce_backward_inplace(use,NULL,w):vsdlss_reduce_forward_inplace(use,w,NULL);
                double took=1000*(now()-start);if(st)return 3;
                int used=vsdlss_parallel_last_team_size();if(used>teams[mode])teams[mode]=used;
                equal[mode]&=memcmp(w,dir?ref:fw,bytes)==0;
                if(k>=0)t[mode][k]=took;
            }
            for(int mode=0;mode<2;mode++){
                qsort(t[mode],(size_t)reps,sizeof(double),cmpd);
                printf("%d,%d,%s,%.6f,%.6f,%.6f,%d,%d\n",nt,mode,dir?"backward":"forward",t[mode][reps/2],t[mode][0],t[mode][reps-1],equal[mode],teams[mode]);
                if(!equal[mode])return 4;
            }fflush(stdout);
        }
    }
    vsdlss_reduction_free(baseline);vsdlss_reduction_free(r);free(b);free(fw);free(ref);free(w);return 0;
}
