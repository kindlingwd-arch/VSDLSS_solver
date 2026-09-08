/* Auto-generated: Ghidra data-type manager (C view). */
#ifndef VSDLSS_TYPES_H
#define VSDLSS_TYPES_H
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include <time.h>
typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned char byte;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef long long longlong;
typedef char * name;

/* recovered composite & enums */
typedef struct {
} __aligned_membuf<anslic_feature*>;

typedef struct {
} __aligned_membuf<CAnsFeatureUsage>;

typedef struct {
} __aligned_membuf<CAnsLicBulkCapRequest>;

typedef struct {
} __aligned_membuf<CAnsLicContextFeatureData>;

typedef struct {
} __aligned_membuf<CAnsServerInfo>;

typedef struct {
} __aligned_membuf<int>;

typedef struct {
} __aligned_membuf<request*>;

typedef struct {
} __aligned_membuf<std::__cxx11::string>;

typedef struct {
} __aligned_membuf<std::pair<ANSLIC_MAX_SET_LIMITS_const,int>>;

typedef struct {
} __aligned_membuf<std::pair<int,CAnsLicLicenseData*>>;

typedef struct {
} __aligned_membuf<std::pair<int,int>>;

typedef struct {
} __aligned_membuf<std::pair<int_const,anslic_feature*>>;

typedef struct {
} __aligned_membuf<std::pair<int_const,std::__cxx11::string>>;

typedef struct {
} __aligned_membuf<std::pair<std::__cxx11::string,int>>;

typedef struct {
} __aligned_membuf<std::pair<std::__cxx11::string,std::__cxx11::string>>;

typedef struct {
} __aligned_membuf<std::pair<std::__cxx11::string_const,anslic_bool>>;

typedef struct {
} __aligned_membuf<std::pair<std::__cxx11::string_const,anslic_client*>>;

typedef struct {
} __aligned_membuf<std::pair<std::__cxx11::string_const,anslic_feature*>>;

typedef struct {
} __aligned_membuf<std::pair<std::__cxx11::string_const,CAnsLicLicenseData*>>;

typedef struct {
} __aligned_membuf<std::pair<std::__cxx11::string_const,request*>>;

typedef struct {
} __aligned_membuf<std::pair<std::__cxx11::string_const,std::__cxx11::list<CAnsFeatureUsage,std::allocator<CAnsFeatureUsage>>>>;

typedef struct {
} __aligned_membuf<std::pair<std::__cxx11::string_const,std::__cxx11::string>>;

typedef struct {
} __aligned_membuf<std::pair<std::__cxx11::string_const,std::map<std::__cxx11::string,std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>>>;

typedef struct {
} __aligned_membuf<std::pair<std::__cxx11::string_const,std::map<void(*)(),void(*)(),std::less<void(*)()>,std::allocator<std::pair<void(*const)(),void(*)()>>>>>;

typedef struct {
} __aligned_membuf<std::pair<void(*const)(),void(*)()>>;

typedef struct {
} __allocated_ptr<std::allocator<std::_List_node<anslic_feature*>>>;

typedef struct {
} __allocated_ptr<std::allocator<std::_List_node<CAnsFeatureUsage>>>;

typedef struct {
} __allocated_ptr<std::allocator<std::_List_node<CAnsLicBulkCapRequest>>>;

typedef struct {
} __allocated_ptr<std::allocator<std::_List_node<CAnsServerInfo>>>;

typedef struct {
} __allocated_ptr<std::allocator<std::_List_node<request*>>>;

typedef struct {
} __allocated_ptr<std::allocator<std::_List_node<std::__cxx11::string>>>;

typedef struct {
} __allocated_ptr<std::allocator<std::_List_node<std::pair<int,CAnsLicLicenseData*>>>>;

typedef struct {
} __allocated_ptr<std::allocator<std::_List_node<std::pair<std::__cxx11::string,int>>>>;

typedef struct {
} __allocated_ptr<std::allocator<std::_List_node<std::pair<std::__cxx11::string,std::__cxx11::string>>>>;

typedef struct {
} __basic_file<char>;

typedef struct {
} __dirstream;

typedef struct {
    /* +0x0 (len 8) */
    int[2] __val;
} __fsid_t;

typedef struct {
    /* +0x0 (len 64) */
    __jmp_buf __jmpbuf;
    /* +0x40 (len 4) */
    int __mask_was_saved;
    /* +0x48 (len 128) */
    __sigset_t __saved_mask;
} __jmp_buf_tag;

typedef struct {
    /* +0x0 (len 8) */
    __pthread_internal_list * __prev;
    /* +0x8 (len 8) */
    __pthread_internal_list * __next;
} __pthread_internal_list;

typedef struct {
    /* +0x0 (len 4) */
    int __lock;
    /* +0x4 (len 4) */
    uint __count;
    /* +0x8 (len 4) */
    int __owner;
    /* +0xc (len 4) */
    uint __nusers;
    /* +0x10 (len 4) */
    int __kind;
    /* +0x14 (len 4) */
    int __spins;
    /* +0x18 (len 16) */
    __pthread_list_t __list;
} __pthread_mutex_s;

typedef int __rlimit_resource; /* enum id __rlimit_resource */

typedef int __rusage_who; /* enum id __rusage_who */

typedef struct {
    /* +0x0 (len 128) */
    ulong[16] __val;
} __sigset_t;

typedef struct {
} __va_list_tag;

typedef struct {
} _Alloc_node;

typedef struct {
} _Alloc_node;

typedef struct {
} _Alloc_node;

typedef struct {
} _Alloc_node;

typedef struct {
} _Alloc_node;

typedef struct {
} _Alloc_node;

typedef struct {
} _Alloc_node;

typedef struct {
} _Alloc_node;

typedef struct {
} _Alloc_node;

typedef struct {
} _Bvector_base<std::allocator<bool>>;

typedef struct {
} _Deque_base<int,std::allocator<int>>;

typedef struct {
} _Identity<std::__cxx11::string>;

typedef struct {
    /* +0x0 (len 4) */
    int _flags;
    /* +0x8 (len 8) */
    char * _IO_read_ptr;
    /* +0x10 (len 8) */
    char * _IO_read_end;
    /* +0x18 (len 8) */
    char * _IO_read_base;
    /* +0x20 (len 8) */
    char * _IO_write_base;
    /* +0x28 (len 8) */
    char * _IO_write_ptr;
    /* +0x30 (len 8) */
    char * _IO_write_end;
    /* +0x38 (len 8) */
    char * _IO_buf_base;
    /* +0x40 (len 8) */
    char * _IO_buf_end;
    /* +0x48 (len 8) */
    char * _IO_save_base;
    /* +0x50 (len 8) */
    char * _IO_backup_base;
    /* +0x58 (len 8) */
    char * _IO_save_end;
    /* +0x60 (len 8) */
    _IO_marker * _markers;
    /* +0x68 (len 8) */
    _IO_FILE * _chain;
    /* +0x70 (len 4) */
    int _fileno;
    /* +0x74 (len 4) */
    int _flags2;
    /* +0x78 (len 8) */
    __off_t _old_offset;
    /* +0x80 (len 2) */
    ushort _cur_column;
    /* +0x82 (len 1) */
    char _vtable_offset;
    /* +0x83 (len 1) */
    char[1] _shortbuf;
    /* +0x88 (len 8) */
    _IO_lock_t * _lock;
    /* +0x90 (len 8) */
    __off64_t _offset;
    /* +0x98 (len 8) */
    void * __pad1;
    /* +0xa0 (len 8) */
    void * __pad2;
    /* +0xa8 (len 8) */
    void * __pad3;
    /* +0xb0 (len 8) */
    void * __pad4;
    /* +0xb8 (len 8) */
    size_t __pad5;
    /* +0xc0 (len 4) */
    int _mode;
    /* +0xc4 (len 20) */
    char[20] _unused2;
} _IO_FILE;

typedef struct {
    /* +0x0 (len 8) */
    _IO_marker * _next;
    /* +0x8 (len 8) */
    _IO_FILE * _sbuf;
    /* +0x10 (len 4) */
    int _pos;
} _IO_marker;

typedef struct {
} _List_base;

typedef struct {
} _List_base<anslic_feature*,std::allocator<anslic_feature*>>;

typedef struct {
} _List_base<CAnsFeatureUsage,std::allocator<CAnsFeatureUsage>>;

typedef struct {
} _List_base<CAnsLicBulkCapRequest,std::allocator<CAnsLicBulkCapRequest>>;

typedef struct {
} _List_base<CAnsLicContextFeatureData,std::allocator<CAnsLicContextFeatureData>>;

typedef struct {
} _List_base<CAnsServerInfo,std::allocator<CAnsServerInfo>>;

typedef struct {
} _List_base<ElementVecBase*,std::allocator<ElementVecBase*>>;

typedef struct {
} _List_base<Engine*,std::allocator<Engine*>>;

typedef struct {
} _List_base<float,std::allocator<float>>;

typedef struct {
} _List_base<int,std::allocator<int>>;

typedef struct {
} _List_base<request*,std::allocator<request*>>;

typedef struct {
} _List_base<ResistorGraph::Locale::Probe,std::allocator<ResistorGraph::Locale::Probe>>;

typedef struct {
} _List_base<std::__cxx11::string,std::allocator<std::__cxx11::string>>;

typedef struct {
} _List_base<std::pair<int,CAnsLicLicenseData*>,std::allocator<std::pair<int,CAnsLicLicenseData*>>>;

typedef struct {
} _List_base<std::pair<int,int>,std::allocator<std::pair<int,int>>>;

typedef struct {
} _List_base<std::pair<std::__cxx11::string,int>,std::allocator<std::pair<std::__cxx11::string,int>>>;

typedef struct {
} _List_base<std::pair<std::__cxx11::string,std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string,std::__cxx11::string>>>;

typedef struct {
} _List_const_iterator<CAnsFeatureUsage>;

typedef struct {
} _List_const_iterator<CAnsLicBulkCapRequest>;

typedef struct {
} _List_const_iterator<CAnsServerInfo>;

typedef struct {
} _List_const_iterator<std::pair<std::__cxx11::string,std::__cxx11::string>>;

typedef struct {
} _List_impl;

typedef struct {
} _List_impl;

typedef struct {
} _List_impl;

typedef struct {
} _List_impl;

typedef struct {
} _List_impl;

typedef struct {
} _List_impl;

typedef struct {
} _List_impl;

typedef struct {
} _List_impl;

typedef struct {
} _List_impl;

typedef struct {
} _List_impl;

typedef struct {
} _List_iterator<anslic_feature*>;

typedef struct {
} _List_iterator<CAnsFeatureUsage>;

typedef struct {
} _List_iterator<CAnsLicBulkCapRequest>;

typedef struct {
} _List_iterator<CAnsLicContextFeatureData>;

typedef struct {
} _List_iterator<CAnsServerInfo>;

typedef struct {
} _List_iterator<request*>;

typedef struct {
} _List_iterator<std::__cxx11::string>;

typedef struct {
} _List_iterator<std::pair<int,CAnsLicLicenseData*>>;

typedef struct {
} _List_iterator<std::pair<int,int>>;

typedef struct {
} _List_iterator<std::pair<std::__cxx11::string,int>>;

typedef struct {
} _List_iterator<std::pair<std::__cxx11::string,std::__cxx11::string>>;

typedef struct {
} _List_node;

typedef struct {
} _List_node<anslic_feature*>;

typedef struct {
} _List_node<CAnsFeatureUsage>;

typedef struct {
} _List_node<CAnsLicBulkCapRequest>;

typedef struct {
} _List_node<CAnsLicContextFeatureData>;

typedef struct {
} _List_node<CAnsServerInfo>;

typedef struct {
} _List_node<int>;

typedef struct {
} _List_node<request*>;

typedef struct {
} _List_node<std::__cxx11::string>;

typedef struct {
} _List_node<std::pair<int,CAnsLicLicenseData*>>;

typedef struct {
} _List_node<std::pair<int,int>>;

typedef struct {
} _List_node<std::pair<std::__cxx11::string,int>>;

typedef struct {
} _List_node<std::pair<std::__cxx11::string,std::__cxx11::string>>;

typedef struct {
} _List_node_base;

typedef struct {
} _List_node_header;

typedef struct {
} _PortCktModel;

typedef struct {
} _Rb_tree;

typedef struct {
} _Rb_tree<ams_ids,std::pair<ams_ids_const,MessageSys::Config>,std::_Select1st<std::pair<ams_ids_const,MessageSys::Config>>,std::less<ams_ids>,std::allocator<std::pair<ams_ids_const,MessageSys::Config>>>;

typedef struct {
} _Rb_tree<ams_ids,std::pair<ams_ids_const,MessageSysEntry>,std::_Select1st<std::pair<ams_ids_const,MessageSysEntry>>,std::less<ams_ids>,std::allocator<std::pair<ams_ids_const,MessageSysEntry>>>;

typedef struct {
} _Rb_tree<ANSLIC_MAX_SET_LIMITS,std::pair<ANSLIC_MAX_SET_LIMITS_const,int>,std::_Select1st<std::pair<ANSLIC_MAX_SET_LIMITS_const,int>>,std::less<ANSLIC_MAX_SET_LIMITS>,std::allocator<std::pair<ANSLIC_MAX_SET_LIMITS_const,int>>>;

typedef struct {
} _Rb_tree<char_const*,std::pair<char_const*const,AplCell>,std::_Select1st<std::pair<char_const*const,AplCell>>,std::less<char_const*>,std::allocator<std::pair<char_const*const,AplCell>>>;

typedef struct {
} _Rb_tree<char_const*,std::pair<char_const*const,int>,std::_Select1st<std::pair<char_const*const,int>>,PackageModel::NameCompare,std::allocator<std::pair<char_const*const,int>>>;

typedef struct {
} _Rb_tree<Connectivity::DomainShort,Connectivity::DomainShort,std::_Identity<Connectivity::DomainShort>,std::less<Connectivity::DomainShort>,std::allocator<Connectivity::DomainShort>>;

typedef struct {
} _Rb_tree<CurrentFile::Read::LightRequest,CurrentFile::Read::LightRequest,std::_Identity<CurrentFile::Read::LightRequest>,std::less<CurrentFile::Read::LightRequest>,std::allocator<CurrentFile::Read::LightRequest>>;

typedef struct {
} _Rb_tree<CurrentFile::Read::Request,CurrentFile::Read::Request,std::_Identity<CurrentFile::Read::Request>,std::less<CurrentFile::Read::Request>,std::allocator<CurrentFile::Read::Request>>;

typedef struct {
} _Rb_tree<CurrentFile::Str,std::pair<CurrentFile::Str_const,int>,std::_Select1st<std::pair<CurrentFile::Str_const,int>>,std::less<CurrentFile::Str>,std::allocator<std::pair<CurrentFile::Str_const,int>>>;

typedef struct {
} _Rb_tree<float,float,std::_Identity<float>,std::less<float>,std::allocator<float>>;

typedef struct {
} _Rb_tree<float,std::pair<float_const,int>,std::_Select1st<std::pair<float_const,int>>,std::less<float>,std::allocator<std::pair<float_const,int>>>;

typedef struct {
} _Rb_tree<Id<Cell>,Id<Cell>,std::_Identity<Id<Cell>>,std::less<Id<Cell>>,std::allocator<Id<Cell>>>;

typedef struct {
} _Rb_tree<Id<Cell>,std::pair<Id<Cell>const,float>,std::_Select1st<std::pair<Id<Cell>const,float>>,std::less<Id<Cell>>,std::allocator<std::pair<Id<Cell>const,float>>>;

typedef struct {
} _Rb_tree<Id<Domain>,std::pair<Id<Domain>const,bool>,std::_Select1st<std::pair<Id<Domain>const,bool>>,std::less<Id<Domain>>,std::allocator<std::pair<Id<Domain>const,bool>>>;

typedef struct {
} _Rb_tree<Id<Instance>,Id<Instance>,std::_Identity<Id<Instance>>,std::less<Id<Instance>>,std::allocator<Id<Instance>>>;

typedef struct {
} _Rb_tree<Id<Instance>,std::pair<Id<Instance>const,float>,std::_Select1st<std::pair<Id<Instance>const,float>>,std::less<Id<Instance>>,std::allocator<std::pair<Id<Instance>const,float>>>;

typedef struct {
} _Rb_tree<Id<Instance>,std::pair<Id<Instance>const,Id<FlopBank>>,std::_Select1st<std::pair<Id<Instance>const,Id<FlopBank>>>,std::less<Id<Instance>>,std::allocator<std::pair<Id<Instance>const,Id<FlopBank>>>>;

typedef struct {
} _Rb_tree<Id<Instance>,std::pair<Id<Instance>const,Id<Instance>>,std::_Select1st<std::pair<Id<Instance>const,Id<Instance>>>,std::less<Id<Instance>>,std::allocator<std::pair<Id<Instance>const,Id<Instance>>>>;

typedef struct {
} _Rb_tree<Id<Node>,Id<Node>,std::_Identity<Id<Node>>,std::less<Id<Node>>,std::allocator<Id<Node>>>;

typedef struct {
} _Rb_tree<Id<Node>,std::pair<Id<Node>const,Id<Node>>,std::_Select1st<std::pair<Id<Node>const,Id<Node>>>,std::less<Id<Node>>,std::allocator<std::pair<Id<Node>const,Id<Node>>>>;

typedef struct {
} _Rb_tree<Id<Node>,std::pair<Id<Node>const,int>,std::_Select1st<std::pair<Id<Node>const,int>>,std::less<Id<Node>>,std::allocator<std::pair<Id<Node>const,int>>>;

typedef struct {
} _Rb_tree<int,int,std::_Identity<int>,std::less<int>,std::allocator<int>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,actPinType>,std::_Select1st<std::pair<int_const,actPinType>>,std::less<int>,std::allocator<std::pair<int_const,actPinType>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,anslic_feature*>,std::_Select1st<std::pair<int_const,anslic_feature*>>,std::less<int>,std::allocator<std::pair<int_const,anslic_feature*>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,aplParSample*>,std::_Select1st<std::pair<int_const,aplParSample*>>,std::less<int>,std::allocator<std::pair<int_const,aplParSample*>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,bool>,std::_Select1st<std::pair<int_const,bool>>,std::less<int>,std::allocator<std::pair<int_const,bool>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,char*>,std::_Select1st<std::pair<int_const,char*>>,std::less<int>,std::allocator<std::pair<int_const,char*>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,float>,std::_Select1st<std::pair<int_const,float>>,std::less<int>,std::allocator<std::pair<int_const,float>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,FlopBankPinRail>,std::_Select1st<std::pair<int_const,FlopBankPinRail>>,std::less<int>,std::allocator<std::pair<int_const,FlopBankPinRail>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,Id<Domain>>,std::_Select1st<std::pair<int_const,Id<Domain>>>,std::less<int>,std::allocator<std::pair<int_const,Id<Domain>>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,Id<LowPowerInst>>,std::_Select1st<std::pair<int_const,Id<LowPowerInst>>>,std::less<int>,std::allocator<std::pair<int_const,Id<LowPowerInst>>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,indexAttr>,std::_Select1st<std::pair<int_const,indexAttr>>,std::less<int>,std::allocator<std::pair<int_const,indexAttr>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,int>,std::_Select1st<std::pair<int_const,int>>,std::less<int>,std::allocator<std::pair<int_const,int>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,std::__cxx11::string>,std::_Select1st<std::pair<int_const,std::__cxx11::string>>,std::less<int>,std::allocator<std::pair<int_const,std::__cxx11::string>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,std::map<int,FlopBankPinRail,std::less<int>,std::allocator<std::pair<int_const,FlopBankPinRail>>>>,std::_Select1st<std::pair<int_const,std::map<int,FlopBankPinRail,std::less<int>,std::allocator<std::pair<int_const,FlopBankPinRail>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::map<int,FlopBankPinRail,std::less<int>,std::allocator<std::pair<int_const,FlopBankPinRail>>>>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,std::map<int,std::map<int,std::vector<aplLayer,std::allocator<aplLayer>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<aplLayer,std::allocator<aplLayer>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::map<int,std::vector<aplLayer,std::allocator<aplLayer>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<aplLayer,std::allocator<aplLayer>>>>>>>>>,std::_Select1st<std::pair<int_const,std::map<int,std::map<int,std::vector<aplLayer,std::allocator<aplLayer>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<aplLayer,std::allocator<aplLayer>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::map<int,std::vector<aplLayer,std::allocator<aplLayer>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<aplLayer,std::allocator<aplLayer>>>>>>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::map<int,std::map<int,std::vector<aplLayer,std::allocator<aplLayer>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<aplLayer,std::allocator<aplLayer>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::map<int,std::vector<aplLayer,std::allocator<aplLayer>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<aplLayer,std::allocator<aplLayer>>>>>>>>>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,std::map<int,std::map<int,std::vector<double,std::allocator<double>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<double,std::allocator<double>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::map<int,std::vector<double,std::allocator<double>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<double,std::allocator<double>>>>>>>>>,std::_Select1st<std::pair<int_const,std::map<int,std::map<int,std::vector<double,std::allocator<double>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<double,std::allocator<double>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::map<int,std::vector<double,std::allocator<double>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<double,std::allocator<double>>>>>>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::map<int,std::map<int,std::vector<double,std::allocator<double>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<double,std::allocator<double>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::map<int,std::vector<double,std::allocator<double>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<double,std::allocator<double>>>>>>>>>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,std::map<int,std::vector<aplLayer,std::allocator<aplLayer>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<aplLayer,std::allocator<aplLayer>>>>>>,std::_Select1st<std::pair<int_const,std::map<int,std::vector<aplLayer,std::allocator<aplLayer>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<aplLayer,std::allocator<aplLayer>>>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::map<int,std::vector<aplLayer,std::allocator<aplLayer>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<aplLayer,std::allocator<aplLayer>>>>>>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,std::map<int,std::vector<double,std::allocator<double>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<double,std::allocator<double>>>>>>,std::_Select1st<std::pair<int_const,std::map<int,std::vector<double,std::allocator<double>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<double,std::allocator<double>>>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::map<int,std::vector<double,std::allocator<double>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<double,std::allocator<double>>>>>>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,std::vector<aplLayer,std::allocator<aplLayer>>>,std::_Select1st<std::pair<int_const,std::vector<aplLayer,std::allocator<aplLayer>>>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<aplLayer,std::allocator<aplLayer>>>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,std::vector<double,std::allocator<double>>>,std::_Select1st<std::pair<int_const,std::vector<double,std::allocator<double>>>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<double,std::allocator<double>>>>>;

typedef struct {
} _Rb_tree<int,std::pair<int_const,std::vector<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>,std::allocator<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>>>>,std::_Select1st<std::pair<int_const,std::vector<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>,std::allocator<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>,std::allocator<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>>>>>>;

typedef struct {
} _Rb_tree<long,std::pair<long_const,std::map<int,std::vector<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>,std::allocator<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>,std::allocator<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>>>>>>>,std::_Select1st<std::pair<long_const,std::map<int,std::vector<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>,std::allocator<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>,std::allocator<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>>>>>>>>,std::less<long>,std::allocator<std::pair<long_const,std::map<int,std::vector<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>,std::allocator<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>,std::allocator<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>>>>>>>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::__cxx11::string,std::_Identity<std::__cxx11::string>,std::less<std::__cxx11::string>,std::allocator<std::__cxx11::string>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,anslic_bool>,std::_Select1st<std::pair<std::__cxx11::string_const,anslic_bool>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,anslic_bool>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,anslic_client*>,std::_Select1st<std::pair<std::__cxx11::string_const,anslic_client*>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,anslic_client*>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,anslic_feature*>,std::_Select1st<std::pair<std::__cxx11::string_const,anslic_feature*>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,anslic_feature*>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,aplPratio*>,std::_Select1st<std::pair<std::__cxx11::string_const,aplPratio*>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,aplPratio*>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,bool>,std::_Select1st<std::pair<std::__cxx11::string_const,bool>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,bool>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,CAnsLicLicenseData*>,std::_Select1st<std::pair<std::__cxx11::string_const,CAnsLicLicenseData*>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,CAnsLicLicenseData*>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,CellPinMap::_cell_info*>,std::_Select1st<std::pair<std::__cxx11::string_const,CellPinMap::_cell_info*>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,CellPinMap::_cell_info*>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,float>,std::_Select1st<std::pair<std::__cxx11::string_const,float>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,float>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,Id<Cell>>,std::_Select1st<std::pair<std::__cxx11::string_const,Id<Cell>>>,NetworkBuild::NameCmp,std::allocator<std::pair<std::__cxx11::string_const,Id<Cell>>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,Id<char>>,std::_Select1st<std::pair<std::__cxx11::string_const,Id<char>>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,Id<char>>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,Id<LdoCell>>,std::_Select1st<std::pair<std::__cxx11::string_const,Id<LdoCell>>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,Id<LdoCell>>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,Id<Pin>>,std::_Select1st<std::pair<std::__cxx11::string_const,Id<Pin>>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,Id<Pin>>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,Id<SWCell>>,std::_Select1st<std::pair<std::__cxx11::string_const,Id<SWCell>>>,NetworkBuild::NameCmp,std::allocator<std::pair<std::__cxx11::string_const,Id<SWCell>>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,int(Options::*)(char_const**)>,std::_Select1st<std::pair<std::__cxx11::string_const,int(Options::*)(char_const**)>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,int(Options::*)(char_const**)>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,int>,std::_Select1st<std::pair<std::__cxx11::string_const,int>>,NetworkBuild::PortNameCompare,std::allocator<std::pair<std::__cxx11::string_const,int>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,int>,std::_Select1st<std::pair<std::__cxx11::string_const,int>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,int>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,long>,std::_Select1st<std::pair<std::__cxx11::string_const,long>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,long>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,NetworkImport::LdoPortTag>,std::_Select1st<std::pair<std::__cxx11::string_const,NetworkImport::LdoPortTag>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,NetworkImport::LdoPortTag>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,Options::Entry*>,std::_Select1st<std::pair<std::__cxx11::string_const,Options::Entry*>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,Options::Entry*>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,request*>,std::_Select1st<std::pair<std::__cxx11::string_const,request*>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,request*>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,std::__cxx11::list<CAnsFeatureUsage,std::allocator<CAnsFeatureUsage>>>,std::_Select1st<std::pair<std::__cxx11::string_const,std::__cxx11::list<CAnsFeatureUsage,std::allocator<CAnsFeatureUsage>>>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::list<CAnsFeatureUsage,std::allocator<CAnsFeatureUsage>>>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,std::__cxx11::list<NetworkBuild::BodyPin,std::allocator<NetworkBuild::BodyPin>>>,std::_Select1st<std::pair<std::__cxx11::string_const,std::__cxx11::list<NetworkBuild::BodyPin,std::allocator<NetworkBuild::BodyPin>>>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::list<NetworkBuild::BodyPin,std::allocator<NetworkBuild::BodyPin>>>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,std::__cxx11::string>,std::_Select1st<std::pair<std::__cxx11::string_const,std::__cxx11::string>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,std::map<int,std::map<int,std::map<int,std::vector<double,std::allocator<double>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<double,std::allocator<double>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::map<int,std::vector<double,std::allocator<double>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<double,std::allocator<double>>>>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::map<int,std::map<int,std::vector<double,std::allocator<double>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<double,std::allocator<double>>>>>,std::less<int>,std::allocator<std::pair<int_const,std::map<int,std::vector<double,std::allocator<double>>,std::less<int>,std::allocator<std::pair<int_const,std::vector<double,std::allocator<double>>>>>>>>>>>>,std::_Select1st<std::pair<std::__cxx11::string_const,std::map<int,std::map<int,std::map<int,std::vector<double,std::allocator<...:less<int>,std::allocator<std::pair<int_const,std::vector<double,std::allocator<double>>>>>>>>>>>>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,std::map<std::__cxx11::string,std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>>,std::_Select1st<std::pair<std::__cxx11::string_const,std::map<std::__cxx11::string,std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::map<std::__cxx11::string,std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,std::map<void(*)(),void(*)(),std::less<void(*)()>,std::allocator<std::pair<void(*const)(),void(*)()>>>>,std::_Select1st<std::pair<std::__cxx11::string_const,std::map<void(*)(),void(*)(),std::less<void(*)()>,std::allocator<std::pair<void(*const)(),void(*)()>>>>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::map<void(*)(),void(*)(),std::less<void(*)()>,std::allocator<std::pair<void(*const)(),void(*)()>>>>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,std::pair<float,float>>,std::_Select1st<std::pair<std::__cxx11::string_const,std::pair<float,float>>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::pair<float,float>>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,std::set<std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::__cxx11::string>>>,std::_Select1st<std::pair<std::__cxx11::string_const,std::set<std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::__cxx11::string>>>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::set<std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::__cxx11::string>>>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string,std::pair<std::__cxx11::string_const,std::vector<int,std::allocator<int>>>,std::_Select1st<std::pair<std::__cxx11::string_const,std::vector<int,std::allocator<int>>>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::vector<int,std::allocator<int>>>>>;

typedef struct {
} _Rb_tree<std::__cxx11::string_const,std::pair<std::__cxx11::string_const,std::__cxx11::string_const>,std::_Select1st<std::pair<std::__cxx11::string_const,std::__cxx11::string_const>>,std::less<std::__cxx11::string_const>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string_const>>>;

typedef struct {
} _Rb_tree<std::pair<Id<Node>,Id<Node>>,std::pair<std::pair<Id<Node>,Id<Node>>const,float>,std::_Select1st<std::pair<std::pair<Id<Node>,Id<Node>>const,float>>,std::less<std::pair<Id<Node>,Id<Node>>>,std::allocator<std::pair<std::pair<Id<Node>,Id<Node>>const,float>>>;

typedef struct {
} _Rb_tree<std::pair<Id<Node>,Id<Node>>,std::pair<std::pair<Id<Node>,Id<Node>>const,Id<EmElement>>,std::_Select1st<std::pair<std::pair<Id<Node>,Id<Node>>const,Id<EmElement>>>,std::less<std::pair<Id<Node>,Id<Node>>>,std::allocator<std::pair<std::pair<Id<Node>,Id<Node>>const,Id<EmElement>>>>;

typedef struct {
} _Rb_tree<std::pair<Id<Node>,Id<Node>>,std::pair<std::pair<Id<Node>,Id<Node>>const,Id<Pad>>,std::_Select1st<std::pair<std::pair<Id<Node>,Id<Node>>const,Id<Pad>>>,std::less<std::pair<Id<Node>,Id<Node>>>,std::allocator<std::pair<std::pair<Id<Node>,Id<Node>>const,Id<Pad>>>>;

typedef struct {
} _Rb_tree<std::pair<Id<Node>,Id<Node>>,std::pair<std::pair<Id<Node>,Id<Node>>const,Id<ProbeNode>>,std::_Select1st<std::pair<std::pair<Id<Node>,Id<Node>>const,Id<ProbeNode>>>,std::less<std::pair<Id<Node>,Id<Node>>>,std::allocator<std::pair<std::pair<Id<Node>,Id<Node>>const,Id<ProbeNode>>>>;

typedef struct {
} _Rb_tree<std::pair<int,int>,std::pair<std::pair<int,int>const,float>,std::_Select1st<std::pair<std::pair<int,int>const,float>>,std::less<std::pair<int,int>>,std::allocator<std::pair<std::pair<int,int>const,float>>>;

typedef struct {
} _Rb_tree<std::pair<int,int>,std::pair<std::pair<int,int>const,prcurelem*>,std::_Select1st<std::pair<std::pair<int,int>const,prcurelem*>>,std::less<std::pair<int,int>>,std::allocator<std::pair<std::pair<int,int>const,prcurelem*>>>;

typedef struct {
} _Rb_tree<unsigned_int,std::pair<unsigned_int_const,unsigned_int>,std::_Select1st<std::pair<unsigned_int_const,unsigned_int>>,std::less<unsigned_int>,std::allocator<std::pair<unsigned_int_const,unsigned_int>>>;

typedef struct {
} _Rb_tree<unsigned_long,std::pair<unsigned_long_const,int>,std::_Select1st<std::pair<unsigned_long_const,int>>,std::less<unsigned_long>,std::allocator<std::pair<unsigned_long_const,int>>>;

typedef struct {
} _Rb_tree<void(*)(),std::pair<void(*const)(),void(*)()>,std::_Select1st<std::pair<void(*const)(),void(*)()>>,std::less<void(*)()>,std::allocator<std::pair<void(*const)(),void(*)()>>>;

typedef struct {
} _Rb_tree_const_iterator<std::__cxx11::string>;

typedef struct {
} _Rb_tree_const_iterator<std::pair<int_const,std::__cxx11::string>>;

typedef struct {
} _Rb_tree_const_iterator<std::pair<std::__cxx11::string_const,anslic_bool>>;

typedef struct {
} _Rb_tree_const_iterator<std::pair<std::__cxx11::string_const,anslic_client*>>;

typedef struct {
} _Rb_tree_const_iterator<std::pair<std::__cxx11::string_const,CAnsLicLicenseData*>>;

typedef struct {
} _Rb_tree_header;

typedef struct {
} _Rb_tree_impl;

typedef struct {
} _Rb_tree_impl<std::less<ANSLIC_MAX_SET_LIMITS>,true>;

typedef struct {
} _Rb_tree_impl<std::less<int>,true>;

typedef struct {
} _Rb_tree_impl<std::less<int>,true>;

typedef struct {
} _Rb_tree_impl<std::less<std::__cxx11::string>,true>;

typedef struct {
} _Rb_tree_impl<std::less<std::__cxx11::string>,true>;

typedef struct {
} _Rb_tree_impl<std::less<std::__cxx11::string>,true>;

typedef struct {
} _Rb_tree_impl<std::less<std::__cxx11::string>,true>;

typedef struct {
} _Rb_tree_impl<std::less<std::__cxx11::string>,true>;

typedef struct {
} _Rb_tree_impl<std::less<std::__cxx11::string>,true>;

typedef struct {
} _Rb_tree_impl<std::less<std::__cxx11::string>,true>;

typedef struct {
} _Rb_tree_impl<std::less<std::__cxx11::string>,true>;

typedef struct {
} _Rb_tree_impl<std::less<std::__cxx11::string>,true>;

typedef struct {
} _Rb_tree_impl<std::less<std::__cxx11::string>,true>;

typedef struct {
} _Rb_tree_impl<std::less<void(*)()>,true>;

typedef struct {
} _Rb_tree_iterator<std::__cxx11::string>;

typedef struct {
} _Rb_tree_iterator<std::pair<ANSLIC_MAX_SET_LIMITS_const,int>>;

typedef struct {
} _Rb_tree_iterator<std::pair<int_const,anslic_feature*>>;

typedef struct {
} _Rb_tree_iterator<std::pair<int_const,std::__cxx11::string>>;

typedef struct {
} _Rb_tree_iterator<std::pair<std::__cxx11::string_const,anslic_bool>>;

typedef struct {
} _Rb_tree_iterator<std::pair<std::__cxx11::string_const,anslic_client*>>;

typedef struct {
} _Rb_tree_iterator<std::pair<std::__cxx11::string_const,anslic_feature*>>;

typedef struct {
} _Rb_tree_iterator<std::pair<std::__cxx11::string_const,CAnsLicLicenseData*>>;

typedef struct {
} _Rb_tree_iterator<std::pair<std::__cxx11::string_const,request*>>;

typedef struct {
} _Rb_tree_iterator<std::pair<std::__cxx11::string_const,std::__cxx11::list<CAnsFeatureUsage,std::allocator<CAnsFeatureUsage>>>>;

typedef struct {
} _Rb_tree_iterator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>;

typedef struct {
} _Rb_tree_iterator<std::pair<std::__cxx11::string_const,std::map<std::__cxx11::string,std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>>>;

typedef struct {
} _Rb_tree_key_compare;

typedef struct {
} _Rb_tree_node;

typedef struct {
} _Rb_tree_node<std::__cxx11::string>;

typedef struct {
} _Rb_tree_node<std::pair<ANSLIC_MAX_SET_LIMITS_const,int>>;

typedef struct {
} _Rb_tree_node<std::pair<int_const,anslic_feature*>>;

typedef struct {
} _Rb_tree_node<std::pair<int_const,std::__cxx11::string>>;

typedef struct {
} _Rb_tree_node<std::pair<std::__cxx11::string_const,anslic_bool>>;

typedef struct {
} _Rb_tree_node<std::pair<std::__cxx11::string_const,anslic_client*>>;

typedef struct {
} _Rb_tree_node<std::pair<std::__cxx11::string_const,anslic_feature*>>;

typedef struct {
} _Rb_tree_node<std::pair<std::__cxx11::string_const,CAnsLicLicenseData*>>;

typedef struct {
} _Rb_tree_node<std::pair<std::__cxx11::string_const,request*>>;

typedef struct {
} _Rb_tree_node<std::pair<std::__cxx11::string_const,std::__cxx11::list<CAnsFeatureUsage,std::allocator<CAnsFeatureUsage>>>>;

typedef struct {
} _Rb_tree_node<std::pair<std::__cxx11::string_const,std::__cxx11::string>>;

typedef struct {
} _Rb_tree_node<std::pair<std::__cxx11::string_const,std::map<std::__cxx11::string,std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>>>;

typedef struct {
} _Rb_tree_node<std::pair<std::__cxx11::string_const,std::map<void(*)(),void(*)(),std::less<void(*)()>,std::allocator<std::pair<void(*const)(),void(*)()>>>>>;

typedef struct {
} _Rb_tree_node<std::pair<void(*const)(),void(*)()>>;

typedef struct {
} _Rb_tree_node_base;

typedef struct {
} _Reuse_or_alloc_node;

typedef struct {
} _Reuse_or_alloc_node;

typedef struct {
} _Reuse_or_alloc_node;

typedef struct {
} _Reuse_or_alloc_node;

typedef struct {
} _Reuse_or_alloc_node;

typedef struct {
} _Select1st<std::pair<ANSLIC_MAX_SET_LIMITS_const,int>>;

typedef struct {
} _Select1st<std::pair<int_const,anslic_feature*>>;

typedef struct {
} _Select1st<std::pair<int_const,std::__cxx11::string>>;

typedef struct {
} _Select1st<std::pair<std::__cxx11::string_const,anslic_bool>>;

typedef struct {
} _Select1st<std::pair<std::__cxx11::string_const,anslic_client*>>;

typedef struct {
} _Select1st<std::pair<std::__cxx11::string_const,anslic_feature*>>;

typedef struct {
} _Select1st<std::pair<std::__cxx11::string_const,CAnsLicLicenseData*>>;

typedef struct {
} _Select1st<std::pair<std::__cxx11::string_const,request*>>;

typedef struct {
} _Select1st<std::pair<std::__cxx11::string_const,std::__cxx11::list<CAnsFeatureUsage,std::allocator<CAnsFeatureUsage>>>>;

typedef struct {
} _Select1st<std::pair<std::__cxx11::string_const,std::__cxx11::string>>;

typedef struct {
} _Select1st<std::pair<std::__cxx11::string_const,std::map<std::__cxx11::string,std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>>>;

typedef struct {
} _Sp_counted_ptr_inplace<apl::ChargeTailCutor,std::allocator<apl::ChargeTailCutor>,(__gnu_cxx::_Lock_policy)2>;

typedef struct {
} _Sp_counted_ptr_inplace<apl::PeakTailCutor,std::allocator<apl::PeakTailCutor>,(__gnu_cxx::_Lock_policy)2>;

typedef struct {
} _Sp_counted_ptr_inplace<apl::RestrictPeakTailCutor,std::allocator<apl::RestrictPeakTailCutor>,(__gnu_cxx::_Lock_policy)2>;

typedef struct {
} _Sp_counted_ptr_inplace<apl::TimeTailCutor,std::allocator<apl::TimeTailCutor>,(__gnu_cxx::_Lock_policy)2>;

typedef struct {
    /* +0x0 (len 4) */
    __pid_t si_pid;
    /* +0x4 (len 4) */
    __uid_t si_uid;
} _struct_1442;

typedef struct {
    /* +0x0 (len 4) */
    int si_tid;
    /* +0x4 (len 4) */
    int si_overrun;
    /* +0x8 (len 8) */
    sigval_t si_sigval;
} _struct_1443;

typedef struct {
    /* +0x0 (len 4) */
    __pid_t si_pid;
    /* +0x4 (len 4) */
    __uid_t si_uid;
    /* +0x8 (len 8) */
    sigval_t si_sigval;
} _struct_1444;

typedef struct {
    /* +0x0 (len 4) */
    __pid_t si_pid;
    /* +0x4 (len 4) */
    __uid_t si_uid;
    /* +0x8 (len 4) */
    int si_status;
    /* +0x10 (len 8) */
    __clock_t si_utime;
    /* +0x18 (len 8) */
    __clock_t si_stime;
} _struct_1445;

typedef struct {
    /* +0x0 (len 8) */
    void * si_addr;
} _struct_1446;

typedef struct {
    /* +0x0 (len 8) */
    long si_band;
    /* +0x8 (len 4) */
    int si_fd;
} _struct_1447;

typedef struct {
    /* +0x0 (len 4) */
    int __lock;
    /* +0x4 (len 4) */
    uint __futex;
    /* +0x8 (len 8) */
    ulonglong __total_seq;
    /* +0x10 (len 8) */
    ulonglong __wakeup_seq;
    /* +0x18 (len 8) */
    ulonglong __woken_seq;
    /* +0x20 (len 8) */
    void * __mutex;
    /* +0x28 (len 4) */
    uint __nwaiters;
    /* +0x2c (len 4) */
    uint __broadcast_seq;
} _struct_16;

typedef struct {
} _Temporary_buffer<__gnu_cxx::__normal_iterator<InstQ01*,std::vector<InstQ01,std::allocator<InstQ01>>>,InstQ01>;

typedef union {
    int[28] _pad;
    _struct_1442 _kill;
    _struct_1443 _timer;
    _struct_1444 _rt;
    _struct_1445 _sigchld;
    _struct_1446 _sigfault;
    _struct_1447 _sigpoll;
} _union_1441;

typedef union {
    __sighandler_t sa_handler;
    _func_5326 * sa_sigaction;
} _union_1457;

typedef struct {
} _Vector_base<_IO_FILE*,std::allocator<_IO_FILE*>>;

typedef struct {
} _Vector_base<char,std::allocator<char>>;

typedef struct {
} _Vector_base<char_const*,std::allocator<char_const*>>;

typedef struct {
} _Vector_base<CurrentFile::IFile::Calc,std::allocator<CurrentFile::IFile::Calc>>;

typedef struct {
} _Vector_base<CurrentFile::Read::LightRequest,std::allocator<CurrentFile::Read::LightRequest>>;

typedef struct {
} _Vector_base<CurrentFile::Read::Request,std::allocator<CurrentFile::Read::Request>>;

typedef struct {
} _Vector_base<CustomSwitchState,std::allocator<CustomSwitchState>>;

typedef struct {
} _Vector_base<double,std::allocator<double>>;

typedef struct {
} _Vector_base<float*,std::allocator<float*>>;

typedef struct {
} _Vector_base<float,std::allocator<float>>;

typedef struct {
} _Vector_base<Id<Instance>,std::allocator<Id<Instance>>>;

typedef struct {
} _Vector_base<Id<Node>,std::allocator<Id<Node>>>;

typedef struct {
} _Vector_base<InstQ01,std::allocator<InstQ01>>;

typedef struct {
} _Vector_base<int,std::allocator<int>>;

typedef struct {
} _Vector_base<long,std::allocator<long>>;

typedef struct {
} _Vector_base<MatrixElement<double,int>,std::allocator<MatrixElement<double,int>>>;

typedef struct {
} _Vector_base<MaxAvgRms,std::allocator<MaxAvgRms>>;

typedef struct {
} _Vector_base<mmxPwlWave*,std::allocator<mmxPwlWave*>>;

typedef struct {
} _Vector_base<Pin,std::allocator<Pin>>;

typedef struct {
} _Vector_base<PinElec,std::allocator<PinElec>>;

typedef struct {
} _Vector_base<RailEffVol::PairV<float>,std::allocator<RailEffVol::PairV<float>>>;

typedef struct {
} _Vector_base<RationalApproxMatrix,std::allocator<RationalApproxMatrix>>;

typedef struct {
} _Vector_base<RawStore<ScenarioCalc::DomainPower>*,std::allocator<RawStore<ScenarioCalc::DomainPower>*>>;

typedef struct {
} _Vector_base<std::complex<double>,std::allocator<std::complex<double>>>;

typedef struct {
} _Vector_base<std::pair<int,int>,std::allocator<std::pair<int,int>>>;

typedef struct {
} _Vector_base<std::pair<long,float>,std::allocator<std::pair<long,float>>>;

typedef struct {
} _Vector_base<SwitchTimeState,std::allocator<SwitchTimeState>>;

typedef struct {
} AbsAverage;

typedef struct {
} actAllCellCdev;

typedef struct {
} actAllCellCdevNew;

typedef struct {
} actAllCellIprof;

typedef struct {
} actAllCellVdCdevNew;

typedef struct {
} actAllSwitch;

typedef struct {
} actCell;

typedef struct {
} actCellCdevNew;

typedef struct {
} actCellIprof;

typedef struct {
} actCellIprofBias;

typedef struct {
} actConstWave;

typedef struct {
} actCorner;

typedef struct {
} actInput;

typedef struct {
} ActionCreate;

typedef struct {
} ActionDestroy;

typedef struct {
} ActionProcess;

typedef struct {
} actIprof;

typedef struct {
} actIprofTag;

typedef struct {
} actParam;

typedef struct {
} actPinCdev;

typedef struct {
} actPulse;

typedef struct {
} actPwlWave;

typedef struct {
} actSample;

typedef struct {
} actswInput;

typedef struct {
} actSwitch;

typedef struct {
} actswMDPWL;

typedef struct {
} actswPWLI;

typedef struct {
} actswPWLR;

typedef struct {
} actswVar;

typedef struct {
} actWave;

typedef struct {
} adsThreadWrapper;

typedef struct {
} allocator;

typedef struct {
} allocator<char>;

typedef struct {
} allocator<std::_List_node<anslic_feature*>>;

typedef struct {
} allocator<std::_List_node<CAnsFeatureUsage>>;

typedef struct {
} allocator<std::_List_node<CAnsLicBulkCapRequest>>;

typedef struct {
} allocator<std::_List_node<CAnsServerInfo>>;

typedef struct {
} allocator<std::_List_node<int>>;

typedef struct {
} allocator<std::_List_node<request*>>;

typedef struct {
} allocator<std::_List_node<std::__cxx11::string>>;

typedef struct {
} allocator<std::_List_node<std::pair<int,CAnsLicLicenseData*>>>;

typedef struct {
} allocator<std::_List_node<std::pair<std::__cxx11::string,int>>>;

typedef struct {
} allocator<std::_List_node<std::pair<std::__cxx11::string,std::__cxx11::string>>>;

typedef struct {
} allocator<std::_Rb_tree_node<std::__cxx11::string>>;

typedef struct {
} allocator<std::_Rb_tree_node<std::pair<ANSLIC_MAX_SET_LIMITS_const,int>>>;

typedef struct {
} allocator<std::_Rb_tree_node<std::pair<int_const,anslic_feature*>>>;

typedef struct {
} allocator<std::_Rb_tree_node<std::pair<int_const,std::__cxx11::string>>>;

typedef struct {
} allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,anslic_bool>>>;

typedef struct {
} allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,anslic_client*>>>;

typedef struct {
} allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,anslic_feature*>>>;

typedef struct {
} allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,CAnsLicLicenseData*>>>;

typedef struct {
} allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,request*>>>;

typedef struct {
} allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,std::__cxx11::list<CAnsFeatureUsage,std::allocator<CAnsFeatureUsage>>>>>;

typedef struct {
} allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>;

typedef struct {
} allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,std::map<std::__cxx11::string,std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>>>>;

typedef struct {
} allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,std::map<void(*)(),void(*)(),std::less<void(*)()>,std::allocator<std::pair<void(*const)(),void(*)()>>>>>>;

typedef struct {
} allocator<std::_Rb_tree_node<std::pair<void(*const)(),void(*)()>>>;

typedef struct {
} ams_crit;

typedef struct {
} ans_mutex;

typedef struct {
} anslic_client;

typedef struct {
} anslic_feature;

typedef struct {
} apl;

typedef struct {
} apl_data_error;

typedef struct {
} apl_data_error_info;

typedef struct {
} apl_imt_file_error;

typedef struct {
} aplAllCellCdev9v2;

typedef struct {
} aplAllCellIprof7v1;

typedef struct {
} aplAllPratio;

typedef struct {
} aplAllVddValues;

typedef struct {
} AplArc;

typedef struct {
} AplCell;

typedef struct {
} aplCellCdev9v2;

typedef struct {
} aplCellIprof7v1;

typedef struct {
} aplCheckLimit;

typedef struct {
} aplChkInput;

typedef struct {
} aplConstWave;

typedef struct {
} AplCurrentModel;

typedef struct {
} AplCustomModel;

typedef struct {
} aplEvent;

typedef struct {
} aplExtParam;

typedef struct {
} aplFileInfo;

typedef struct {
} AplFunction;

typedef struct {
} aplFunMemPool;

typedef struct {
} aplInput;

typedef struct {
} aplIPeaks;

typedef struct {
} aplLayer;

typedef struct {
} aplLayerInfo;

typedef struct {
} AplLibrary;

typedef struct {
} aplMergeParam;

typedef struct {
} aplMultiToggle;

typedef struct {
} aplNewAllPratio;

typedef struct {
} aplNewPinsXYPratio;

typedef struct {
} aplParSample;

typedef struct {
} aplPwlWave;

typedef struct {
} aplPwlWaveDt;

typedef struct {
} aplState;

typedef struct {
} aplStateToggle;

typedef struct {
} aplSwHeader;

typedef struct {
} AplTailCut;

typedef struct {
} AplTailCutContext;

typedef struct {
} aplTiming;

typedef struct {
} aplToggle;

typedef struct {
} aplTriWave;

typedef struct {
} AplVcdCurrentModel;

typedef struct {
} AplVcdCustomModel;

typedef struct {
} AplVcdMerge;

typedef struct {
} AplWave;

typedef struct {
} aplWave;

typedef struct {
} ApproxResult;

typedef struct {
} Arc;

typedef struct {
} ArcData;

typedef struct {
} ArcDataInternal;

typedef struct {
} ArcMCycleData;

typedef struct {
} ArcsIter;

typedef struct {
} ArrayData<AbsAverage>;

typedef struct {
} ArrayData<bool>;

typedef struct {
} ArrayData<char>;

typedef struct {
} ArrayData<Cycle>;

typedef struct {
} ArrayData<CycleComplete::Result>;

typedef struct {
} ArrayData<double>;

typedef struct {
} ArrayData<DvdGlitch>;

typedef struct {
} ArrayData<EmEltAlert>;

typedef struct {
} ArrayData<float>;

typedef struct {
} ArrayData<int>;

typedef struct {
} ArrayData<MaxAvgRms>;

typedef struct {
} ArrayData<MinAveMax<float,double>>;

typedef struct {
} ArrayData<MinMax<float>>;

typedef struct {
} ArrayData<short>;

typedef struct {
} ArrayData<std::pair<int,int>>;

typedef struct {
} ArrayData<std::vector<double,std::allocator<double>>>;

typedef struct {
} ArrayData<std::vector<float,std::allocator<float>>>;

typedef struct {
} ArrayData<unsigned_short>;

typedef struct {
} ArrayData<Window>;

typedef struct {
} ArrayData<WindowRms>;

typedef struct {
} ArrayData<WorstEvdd>;

typedef struct {
} ArrayData<WorstPinv>;

typedef struct {
} AsimTimers;

typedef struct {
} AutoPresim;

typedef struct {
} bad_alloc;

typedef struct {
} bad_cast;

typedef struct {
} bad_function_call;

typedef struct {
} bad_lexical_cast;

typedef struct {
} basic_pointerbuf<char,std::streambuf>;

typedef struct {
} basic_unlockedbuf<std::streambuf,char>;

typedef struct {
} BiasPin;

typedef struct {
} BigArcVoltages;

typedef struct {
} BigPinVoltages;

typedef struct {
} Block;

typedef struct {
} Bucket;

typedef struct {
} Buffer;

typedef struct {
} CacheFile;

typedef struct {
} CAclClient;

typedef struct {
} Calculator;

typedef struct {
} CalFrqdCurrent;

typedef struct {
} CAnsAcademicInfo;

typedef struct {
} CAnsFeatureUsage;

typedef struct {
} CAnsLicBulkCapRequest;

typedef struct {
} CAnsLicContext;

typedef struct {
} CAnsLicContextData;

typedef struct {
} CAnsLicContextFeatureData;

typedef struct {
} CAnsLicException;

typedef struct {
} CAnsLicLicenseData;

typedef struct {
} CAnsLicMessages;

typedef struct {
} CAnsServerInfo;

typedef struct {
} CAnsStringUtilities;

typedef struct {
} CapTotals;

typedef struct {
} CasiSolver<int>;

typedef struct {
} CasiSolver<long>;

typedef struct {
} cBlockQR;

typedef struct {
} cccs;

typedef struct {
} cColumnNormEnforcer;

typedef struct {
} cCommonPoleFit;

typedef struct {
} ccvs;

typedef struct {
} cElementCorrector;

typedef struct {
} Cell;

typedef struct {
} cell;

typedef struct {
} CellData;

typedef struct {
} CellDataInternal;

typedef struct {
} CellPinMap;

typedef struct {
} cellrpt;

typedef struct {
} CentralFreeList;

typedef struct {
} ChargeTailCutor;

typedef struct {
} chkError;

typedef struct {
} ClockNetworkSwitching;

typedef struct {
} cMatrixFit;

typedef struct {
} Column<double,int>;

typedef struct {
} CombCurrentModel;

typedef struct {
} complex;

typedef struct {
} CompressedVFile;

typedef struct {
} Config;

typedef struct {
} Connectivity;

typedef struct {
} Convert<double,float>;

typedef struct {
} cPassivityEnforcer;

typedef struct {
} cPassivityViolation;

typedef struct {
} Cpm;

typedef struct {
} cRedhawkPkg;

typedef struct {
} CrossSectMerge;

typedef struct {
} cSmatrixFit;

typedef struct {
} ctype;

typedef struct {
} ctype<char>;

typedef struct {
} CurrentModel;

typedef struct {
} CurrentModelList;

typedef struct {
} CurrentModelParams;

typedef struct {
} CurrentModelPostProcess;

typedef struct {
} CurrentOnlyPostProcesses;

typedef struct {
} CustomFFIdleSwitching;

typedef struct {
} CustomFFToggleSwitching;

typedef struct {
} CustomRandomSwitching<ExtPermSwitchVec>;

typedef struct {
} CustomRandomSwitching<TmpSwitchVec>;

typedef struct {
} CustomSwitchState;

typedef struct {
} CustomToggleSwitching;

typedef struct {
} Cycle;

typedef struct {
} CycleComplete;

typedef struct {
} CycleCompute;

typedef struct {
} CycleTest;

typedef struct {
} Data;

typedef struct {
} DCap;

typedef struct {
} DebugOutput;

typedef struct {
} DebugOutputSelected;

typedef struct {
} DebugOutputTime;

typedef struct {
} DecapInst;

typedef struct {
} DecapPinElec;

typedef struct {
} DecapSwitching;

typedef struct {
} DefaultSysAllocator;

typedef struct {
} DelayedPostProcesses;

typedef struct {
} Dense<double,int>;

typedef struct {
} deque<ElemSubMdl*,std::allocator<ElemSubMdl*>>;

typedef struct {
} deque<int,std::allocator<int>>;

typedef struct {
} deque<std::__cxx11::string,std::allocator<std::__cxx11::string>>;

typedef struct {
} DesignCorner;

typedef struct {
} DetachedPostProcesses;

typedef struct {
} DevMemSysAllocator;

typedef struct {
} devpwcap;

typedef struct {
} devpwcapnew;

typedef struct {
} DFile;

typedef struct {
} DieInfo;

typedef struct {
    /* +0x0 (len 8) */
    __ino_t d_ino;
    /* +0x8 (len 8) */
    __off_t d_off;
    /* +0x10 (len 2) */
    ushort d_reclen;
    /* +0x12 (len 1) */
    uchar d_type;
    /* +0x13 (len 256) */
    char[256] d_name;
} dirent;

typedef struct {
} DiskFullAlert;

typedef struct {
} DmpNodeMap;

typedef struct {
} DmpPinNodeMap;

typedef struct {
} DomainPower;

typedef struct {
} DomainShort;

typedef struct {
} DotReminder;

typedef struct {
} doublecomplex;

typedef struct {
} DumpDmpStats;

typedef struct {
} DumpDmpVecs;

typedef struct {
} DumpSwitchCurrents;

typedef struct {
} DumpVddRanges;

typedef struct {
} DvdGlitch;

typedef struct {
} DvdGlitchFilter;

typedef struct {
} DvdGlitchTracker;

typedef struct {
} DvdTraceNodes;

typedef struct {
} DynamicSwitchReports;

typedef struct {
} Edge;

typedef struct {
} EdgeRes;

typedef struct {
    /* +0x0 (len 1) */
    byte eh_frame_hdr_version;
    /* +0x1 (len 1) */
    dwfenc eh_frame_pointer_encoding;
    /* +0x2 (len 1) */
    dwfenc eh_frame_desc_entry_count_encoding;
    /* +0x3 (len 1) */
    dwfenc eh_frame_table_encoding;
} eh_frame_hdr;

typedef struct {
} einstance;

typedef struct {
} Elem;

typedef struct {
} ElemCCS;

typedef struct {
} ElementVec<CcCs,CcCs>;

typedef struct {
} ElementVec<CcVs,CcVs>;

typedef struct {
} ElementVec<DCap,DCap>;

typedef struct {
} ElementVec<DecapInst,DecapInst>;

typedef struct {
} ElementVec<FosterVcCs,FosterVcCs>;

typedef struct {
} ElementVec<Glitch,Glitch>;

typedef struct {
} ElementVec<ICap,ICap>;

typedef struct {
} ElementVec<Impedance,Impedance>;

typedef struct {
} ElementVec<Inductor,Inductor>;

typedef struct {
} ElementVec<InitNodeV,InitNodeV>;

typedef struct {
} ElementVec<Instance,Instance>;

typedef struct {
} ElementVec<IPad,IPad>;

typedef struct {
} ElementVec<ISource,ISource>;

typedef struct {
} ElementVec<LdoInst,LdoInst>;

typedef struct {
} ElementVec<LdoPin,LdoPin>;

typedef struct {
} ElementVec<LGroup,LGroup>;

typedef struct {
} ElementVec<LowPowerInst,LowPowerInst>;

typedef struct {
} ElementVec<Node,Node>;

typedef struct {
} ElementVec<NoiseResistor,NoiseResistor>;

typedef struct {
} ElementVec<Pad,Pad>;

typedef struct {
} ElementVec<Pin,Pin>;

typedef struct {
} ElementVec<PiNode,PiNode>;

typedef struct {
} ElementVec<PinStamp,PinStamp>;

typedef struct {
} ElementVec<PSimPad,PSimPad>;

typedef struct {
} ElementVec<Pwl2DVcCs,Pwl2DVcCs>;

typedef struct {
} ElementVec<PwlCap,PwlCap>;

typedef struct {
} ElementVec<PwlCcVs,PwlCcVs>;

typedef struct {
} ElementVec<PwlImodel,PwlImodel>;

typedef struct {
} ElementVec<PwlIsrc,PwlIsrc>;

typedef struct {
} ElementVec<PwlVsrc,PwlVsrc>;

typedef struct {
} ElementVec<Resistor,Resistor>;

typedef struct {
} ElementVec<SeriesRC,SeriesRC>;

typedef struct {
} ElementVec<SnapBackDevice,SnapBackDevice>;

typedef struct {
} ElementVec<Tvr,Tvr>;

typedef struct {
} ElementVec<VcCs,VcCs>;

typedef struct {
} ElementVec<VcVs,VcVs>;

typedef struct {
} ElementVec<Vs,Vs>;

typedef struct {
} ElementVec<VSource,VSource>;

typedef struct {
} ElemIc;

typedef struct {
} ElemInd;

typedef struct {
} ElemK;

typedef struct {
} ElemNPort;

typedef struct {
} ElemNPortMdl;

typedef struct {
} ElemPrI;

typedef struct {
} ElemPrV;

typedef struct {
} ElemPulse;

typedef struct {
} ElemPwl;

typedef struct {
} ElemPwl2VCCS;

typedef struct {
} ElemPwlCCVS;

typedef struct {
} ElemPwlVCVS;

typedef struct {
} Elems;

typedef struct {
} ElemSin;

typedef struct {
} ElemSub;

typedef struct {
} ElemSubMdl;

typedef struct {
} ElemVCS;

typedef struct {
    /* +0x0 (len 8) */
    Elf64_DynTag d_tag;
    /* +0x8 (len 8) */
    qword d_val;
} Elf64_Dyn;

typedef int Elf64_DynTag; /* enum id Elf64_DynTag */

typedef struct {
    /* +0x0 (len 1) */
    byte e_ident_magic_num;
    /* +0x1 (len 3) */
    string e_ident_magic_str;
    /* +0x4 (len 1) */
    byte e_ident_class;
    /* +0x5 (len 1) */
    byte e_ident_data;
    /* +0x6 (len 1) */
    byte e_ident_version;
    /* +0x7 (len 1) */
    byte e_ident_osabi;
    /* +0x8 (len 1) */
    byte e_ident_abiversion;
    /* +0x9 (len 7) */
    byte[7] e_ident_pad;
    /* +0x10 (len 2) */
    word e_type;
    /* +0x12 (len 2) */
    word e_machine;
    /* +0x14 (len 4) */
    dword e_version;
    /* +0x18 (len 8) */
    qword e_entry;
    /* +0x20 (len 8) */
    qword e_phoff;
    /* +0x28 (len 8) */
    qword e_shoff;
    /* +0x30 (len 4) */
    dword e_flags;
    /* +0x34 (len 2) */
    word e_ehsize;
    /* +0x36 (len 2) */
    word e_phentsize;
    /* +0x38 (len 2) */
    word e_phnum;
    /* +0x3a (len 2) */
    word e_shentsize;
    /* +0x3c (len 2) */
    word e_shnum;
    /* +0x3e (len 2) */
    word e_shstrndx;
} Elf64_Ehdr;

typedef struct {
    /* +0x0 (len 4) */
    Elf_ProgramHeaderType p_type;
    /* +0x4 (len 4) */
    dword p_flags;
    /* +0x8 (len 8) */
    qword p_offset;
    /* +0x10 (len 8) */
    qword p_vaddr;
    /* +0x18 (len 8) */
    qword p_paddr;
    /* +0x20 (len 8) */
    qword p_filesz;
    /* +0x28 (len 8) */
    qword p_memsz;
    /* +0x30 (len 8) */
    qword p_align;
} Elf64_Phdr;

typedef struct {
    /* +0x0 (len 8) */
    qword r_offset;
    /* +0x8 (len 8) */
    qword r_info;
    /* +0x10 (len 8) */
    qword r_addend;
} Elf64_Rela;

typedef struct {
    /* +0x0 (len 4) */
    dword sh_name;
    /* +0x4 (len 4) */
    Elf_SectionHeaderType sh_type;
    /* +0x8 (len 8) */
    qword sh_flags;
    /* +0x10 (len 8) */
    qword sh_addr;
    /* +0x18 (len 8) */
    qword sh_offset;
    /* +0x20 (len 8) */
    qword sh_size;
    /* +0x28 (len 4) */
    dword sh_link;
    /* +0x2c (len 4) */
    dword sh_info;
    /* +0x30 (len 8) */
    qword sh_addralign;
    /* +0x38 (len 8) */
    qword sh_entsize;
} Elf64_Shdr;

typedef struct {
    /* +0x0 (len 4) */
    dword st_name;
    /* +0x4 (len 1) */
    byte st_info;
    /* +0x5 (len 1) */
    byte st_other;
    /* +0x6 (len 2) */
    word st_shndx;
    /* +0x8 (len 8) */
    qword st_value;
    /* +0x10 (len 8) */
    qword st_size;
} Elf64_Sym;

typedef int Elf_ProgramHeaderType; /* enum id Elf_ProgramHeaderType */

typedef int Elf_SectionHeaderType; /* enum id Elf_SectionHeaderType */

typedef struct {
} EltsIterator;

typedef struct {
} EltsIterator;

typedef struct {
} EmElement;

typedef struct {
} EmEltAlert;

typedef struct {
} EmiPartition;

typedef struct {
} EmRuleSpec;

typedef struct {
} Engine;

typedef struct {
} EngineGraph;

typedef struct {
} EngineThread;

typedef struct {
} Entry;

typedef struct {
} Error;

typedef struct {
} EsdDiodeIpeak;

typedef struct {
} ESDModel;

typedef struct {
} EsdStressCheck;

typedef struct {
} evp_pkey_ctx_st;

typedef struct {
} exception;

typedef struct {
} exception;

typedef struct {
} expFunction;

typedef struct {
} expInfo;

typedef struct {
} expOperator;

typedef struct {
} expParam;

typedef struct {
} expTree;

typedef struct {
} expValue;

typedef struct {
} ExtPermSwitchVec;

typedef struct {
    /* +0x0 (len 128) */
    __fd_mask[16] fds_bits;
} fd_set;

typedef struct {
    /* +0x0 (len 4) */
    dword initial_loc;
    /* +0x4 (len 4) */
    dword data_loc;
} fde_table_entry;

typedef struct {
} ffbankPinIPeak;

typedef struct {
} FFCurrentModel;

typedef struct {
} FFIdleSwitching;

typedef struct {
} FFToggleSwitching;

typedef struct {
} File;

typedef struct {
} filebuf;

typedef struct {
} FileGzip;

typedef struct {
} FileGzip2;

typedef struct {
} FileMap;

typedef struct {
} FileNormal;

typedef struct {
} FilterPara;

typedef struct {
} first_type;

typedef struct {
} first_type;

typedef struct {
} first_type;

typedef struct {
} first_type;

typedef struct {
} first_type;

typedef struct {
} first_type;

typedef struct {
} FlopBank;

typedef struct {
} FlopBankBitElec;

typedef struct {
} FlopBankRail;

typedef struct {
} FortranComplexMatrix;

typedef struct {
} FortranComplexVector;

typedef struct {
} FortranDoubleMatrix;

typedef struct {
} FortranDoubleVector;

typedef struct {
} foster;

typedef struct {
} FosterVcCs;

typedef struct {
} FrameEmStats;

typedef struct {
} FreeList;

typedef struct {
} FreeListInfo;

typedef struct {
} function2<boost::iterator_range<__gnu_cxx::__normal_iterator<char*,std::__cxx11::string>>,__gnu_cxx::__normal_iterator<char*,std::__cxx11::string>,__gnu_cxx::__normal_iterator<char*,std::__cxx11::string>>;

typedef struct {
} function2<boost::iterator_range<__gnu_cxx::__normal_iterator<char_const*,std::__cxx11::string>>,__gnu_cxx::__normal_iterator<char_const*,std::__cxx11::string>,__gnu_cxx::__normal_iterator<char_const*,std::__cxx11::string>>;

typedef struct {
} function_buffer;

typedef struct {
} Functor1;

typedef struct {
} Functor2;

typedef struct {
} g3DES;

typedef struct {
} g3DES2;

typedef struct {
} GA1DBinaryStringGenome;

typedef struct {
} GABin2DecGenome;

typedef struct {
} GABin2DecPhenotype;

typedef struct {
} GABin2DecPhenotypeCore;

typedef struct {
} GABinaryString;

typedef struct {
} GADSSelector;

typedef struct {
} GAEvalData;

typedef struct {
} GAGeneticAlgorithm;

typedef struct {
} GAGenome;

typedef struct {
} GALinearScaling;

typedef struct {
} GANoScaling;

typedef struct {
} GAParameter;

typedef struct {
} GAParameterList;

typedef struct {
} GAPopulation;

typedef struct {
} GAPowerLawScaling;

typedef struct {
} GARankSelector;

typedef struct {
} GARouletteWheelSelector;

typedef struct {
} GAScalingScheme;

typedef struct {
} GASelectionScheme;

typedef struct {
} GASharing;

typedef struct {
} GASigmaTruncationScaling;

typedef struct {
} GASimpleGA;

typedef struct {
} GASRSSelector;

typedef struct {
} GAStatistics;

typedef struct {
} GATournamentSelector;

typedef struct {
} GAUniformSelector;

typedef struct {
} gBigNumber;

typedef struct {
} gBigNumberMont;

typedef struct {
} gDES;

typedef struct {
} General;

typedef struct {
} General<double,int,false>;

typedef struct {
} General<double,int,true>;

typedef struct {
} GenerateInstPowerModel;

typedef struct {
} Glitch;

typedef struct {
} gRSA;

typedef struct {
} gRSAPrivateKey;

typedef struct {
} gRSAPublicKey;

typedef struct {
} gzFile_s;

typedef struct {
} HashMap;

typedef struct {
} HashSet<HashMap<char_const*,Id<Node>,Hash<char_const*>,Comp<char_const*>>::Pair,HashMap<char_const*,Id<Node>,Hash<char_const*>,Comp<char_const*>>::PHash,HashMap<char_const*,Id<Node>,Hash<char_const*>,Comp<char_const*>>::PComp>;

typedef struct {
} HashSet<HashMap<char_const*,Id<ReportLdoResult::Wave>,Hash<char_const*>,Comp<char_const*>>::Pair,HashMap<char_const*,Id<ReportLdoResult::Wave>,Hash<char_const*>,Comp<char_const*>>::PHash,HashMap<char_const*,Id<ReportLdoResult::Wave>,Hash<char_const*>,Comp<char_const*>>::PComp>;

typedef struct {
} HashSet<HashMap<char_const*,unsigned_int,Hash<char_const*>,Comp<char_const*>>::Pair,HashMap<char_const*,unsigned_int,Hash<char_const*>,Comp<char_const*>>::PHash,HashMap<char_const*,unsigned_int,Hash<char_const*>,Comp<char_const*>>::PComp>;

typedef struct {
} HashSet<HashMap<Id<Instance>,Id<short>,ScenarioCalc::HashId<Instance>,Comp<Id<Instance>>>::Pair,HashMap<Id<Instance>,Id<short>,ScenarioCalc::HashId<Instance>,Comp<Id<Instance>>>::PHash,HashMap<Id<Instance>,Id<short>,ScenarioCalc::HashId<Instance>,Comp<Id<Instance>>>::PComp>;

typedef struct {
} HashSet<HashMap<Id<Instance>,Id<std::__cxx11::string>,HashId<Instance>,Comp<Id<Instance>>>::Pair,HashMap<Id<Instance>,Id<std::__cxx11::string>,HashId<Instance>,Comp<Id<Instance>>>::PHash,HashMap<Id<Instance>,Id<std::__cxx11::string>,HashId<Instance>,Comp<Id<Instance>>>::PComp>;

typedef struct {
} HashSet<HashMap<std::__cxx11::string,Id<Instance>,Hash<std::__cxx11::string>,Comp<std::__cxx11::string>>::Pair,HashMap<std::__cxx11::string,Id<Instance>,Hash<std::__cxx11::string>,Comp<std::__cxx11::string>>::PHash,HashMap<std::__cxx11::string,Id<Instance>,Hash<std::__cxx11::string>,Comp<std::__cxx11::string>>::PComp>;

typedef struct {
} HashSet<HashMap<std::__cxx11::string,Id<Tvr>,Hash<std::__cxx11::string>,Comp<std::__cxx11::string>>::Pair,HashMap<std::__cxx11::string,Id<Tvr>,Hash<std::__cxx11::string>,Comp<std::__cxx11::string>>::PHash,HashMap<std::__cxx11::string,Id<Tvr>,Hash<std::__cxx11::string>,Comp<std::__cxx11::string>>::PComp>;

typedef struct {
} HashSet<HashMap<StrLen,bool,Hash<StrLen>,Comp<StrLen>>::Pair,HashMap<StrLen,bool,Hash<StrLen>,Comp<StrLen>>::PHash,HashMap<StrLen,bool,Hash<StrLen>,Comp<StrLen>>::PComp>;

typedef struct {
} HashSet<HashMap<StrLen,Id<Instance>,Hash<StrLen>,Comp<StrLen>>::Pair,HashMap<StrLen,Id<Instance>,Hash<StrLen>,Comp<StrLen>>::PHash,HashMap<StrLen,Id<Instance>,Hash<StrLen>,Comp<StrLen>>::PComp>;

typedef struct {
} HashSet<HashMap<unsigned_int,Id<Instance>,Hash<unsigned_int>,Comp<unsigned_int>>::Pair,HashMap<unsigned_int,Id<Instance>,Hash<unsigned_int>,Comp<unsigned_int>>::PHash,HashMap<unsigned_int,Id<Instance>,Hash<unsigned_int>,Comp<unsigned_int>>::PComp>;

typedef struct {
} HookList<int(*)(void_const*,unsigned_long,int*)>;

typedef struct {
} HookList<int(*)(void_const*,unsigned_long,int,int,int,long,void**)>;

typedef struct {
} HookList<void(*)(long)>;

typedef struct {
} HookList<void(*)(void_const*)>;

typedef struct {
} HookList<void(*)(void_const*,long)>;

typedef struct {
} HookList<void(*)(void_const*,unsigned_long)>;

typedef struct {
} HookList<void(*)(void_const*,unsigned_long,int,int,int,long)>;

typedef struct {
} HookList<void(*)(void_const*,void_const*,unsigned_long,int,int,int,long)>;

typedef struct {
} HookList<void(*)(void_const*,void_const*,unsigned_long,unsigned_long,int,void_const*)>;

typedef struct {
} i_cap;

typedef struct {
} ICap;

typedef struct {
} IdleSwitching;

typedef struct {
} IdWt;

typedef struct {
} IFile;

typedef struct {
} IFileCrypt;

typedef struct {
} IFileCryptGzip;

typedef struct {
} ifstream;

typedef struct {
} ImapFile;

typedef struct {
} IndEmStats;

typedef struct {
} indexAttr;

typedef struct {
} IndexedMerge<float,unsigned_short,double>;

typedef struct {
} IndexSortval;

typedef struct {
} Inductor;

typedef struct {
} inductor;

typedef struct {
} INetwork;

typedef struct {
} Init;

typedef struct {
} initnodevol;

typedef struct {
} Instance;

typedef struct {
} InstanceElec;

typedef struct {
} InstanceFinder;

typedef struct {
} InstData;

typedef struct {
} InstData;

typedef struct {
} InstDataInternal;

typedef struct {
} InstIdentData;

typedef struct {
} InstPowerModel;

typedef struct {
} InstQ01;

typedef struct {
} InstSwitching;

typedef struct {
} InstTiming;

typedef struct {
} InstWstEffData;

typedef struct {
} InterleavedInstSwitching;

typedef struct {
} invalid_argument;

typedef struct {
} ios_base;

typedef struct {
} iostream;

typedef struct {
    /* +0x0 (len 4) */
    __key_t __key;
    /* +0x4 (len 4) */
    __uid_t uid;
    /* +0x8 (len 4) */
    __gid_t gid;
    /* +0xc (len 4) */
    __uid_t cuid;
    /* +0x10 (len 4) */
    __gid_t cgid;
    /* +0x14 (len 2) */
    ushort mode;
    /* +0x16 (len 2) */
    ushort __pad1;
    /* +0x18 (len 2) */
    ushort __seq;
    /* +0x1a (len 2) */
    ushort __pad2;
    /* +0x20 (len 8) */
    ulong __unused1;
    /* +0x28 (len 8) */
    ulong __unused2;
} ipc_perm;

typedef struct {
} iprofile;

typedef struct {
} isource;

typedef struct {
} istream;

typedef struct {
} istringstream;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} Iterator;

typedef struct {
} IvddVsrcFormat;

typedef struct {
} keyWrapper;

typedef struct {
} LargeSpanStats;

typedef struct {
} LCurrentStats;

typedef struct {
} LCurrentStatsDMP;

typedef struct {
} LdoInst;

typedef struct {
} LdoPin;

typedef struct {
} LdoPort;

typedef struct {
} less;

typedef struct {
} less<ANSLIC_MAX_SET_LIMITS>;

typedef struct {
} less<int>;

typedef struct {
} less<std::__cxx11::string>;

typedef struct {
} LGroup;

typedef struct {
} libpwcap;

typedef struct {
} libpwcapnew;

typedef struct {
} lic_exception;

typedef struct {
} LightRequest;

typedef struct {
} list;

typedef struct {
} list<anslic_feature*,std::allocator<anslic_feature*>>;

typedef struct {
} list<CAnsFeatureUsage,std::allocator<CAnsFeatureUsage>>;

typedef struct {
} list<CAnsLicBulkCapRequest,std::allocator<CAnsLicBulkCapRequest>>;

typedef struct {
} list<CAnsLicContextFeatureData,std::allocator<CAnsLicContextFeatureData>>;

typedef struct {
} list<CAnsServerInfo,std::allocator<CAnsServerInfo>>;

typedef struct {
} list<int,std::allocator<int>>;

typedef struct {
} list<PagedVecBase*,std::allocator<PagedVecBase*>>;

typedef struct {
} list<request*,std::allocator<request*>>;

typedef struct {
} list<std::__cxx11::string,std::allocator<std::__cxx11::string>>;

typedef struct {
} list<std::pair<int,CAnsLicLicenseData*>,std::allocator<std::pair<int,CAnsLicLicenseData*>>>;

typedef struct {
} list<std::pair<int,int>,std::allocator<std::pair<int,int>>>;

typedef struct {
} list<std::pair<std::__cxx11::string,int>,std::allocator<std::pair<std::__cxx11::string,int>>>;

typedef struct {
} list<std::pair<std::__cxx11::string,std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string,std::__cxx11::string>>>;

typedef struct {
} ListValue<float>;

typedef struct {
} ListValue<std::__cxx11::string>;

typedef struct {
} LoadTypes;

typedef struct {
} Locale;

typedef struct {
} locale;

typedef struct {
} Logger;

typedef struct {
} LogItem;

typedef struct {
} LowPowerInst;

typedef struct {
} LowPowerInstIdMap;

typedef struct {
} MallocExtension;

typedef struct {
} MallocRange;

typedef struct {
} map;

typedef struct {
} map<ANSLIC_MAX_SET_LIMITS,int,std::less<ANSLIC_MAX_SET_LIMITS>,std::allocator<std::pair<ANSLIC_MAX_SET_LIMITS_const,int>>>;

typedef struct {
} map<Id<Node>,Id<Node>,std::less<Id<Node>>,std::allocator<std::pair<Id<Node>const,Id<Node>>>>;

typedef struct {
} map<Id<Node>,int,std::less<Id<Node>>,std::allocator<std::pair<Id<Node>const,int>>>;

typedef struct {
} map<int,anslic_feature*,std::less<int>,std::allocator<std::pair<int_const,anslic_feature*>>>;

typedef struct {
} map<int,aplParSample*,std::less<int>,std::allocator<std::pair<int_const,aplParSample*>>>;

typedef struct {
} map<int,std::__cxx11::string,std::less<int>,std::allocator<std::pair<int_const,std::__cxx11::string>>>;

typedef struct {
} map<std::__cxx11::string,anslic_bool,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,anslic_bool>>>;

typedef struct {
} map<std::__cxx11::string,anslic_client*,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,anslic_client*>>>;

typedef struct {
} map<std::__cxx11::string,anslic_feature*,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,anslic_feature*>>>;

typedef struct {
} map<std::__cxx11::string,CAnsLicLicenseData*,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,CAnsLicLicenseData*>>>;

typedef struct {
} map<std::__cxx11::string,request*,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,request*>>>;

typedef struct {
} map<std::__cxx11::string,std::__cxx11::list<CAnsFeatureUsage,std::allocator<CAnsFeatureUsage>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::list<CAnsFeatureUsage,std::allocator<CAnsFeatureUsage>>>>>;

typedef struct {
} map<std::__cxx11::string,std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>;

typedef struct {
} map<std::__cxx11::string,std::map<std::__cxx11::string,std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::map<std::__cxx11::string,std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>>>>;

typedef struct {
} map<std::__cxx11::string,std::map<void(*)(),void(*)(),std::less<void(*)()>,std::allocator<std::pair<void(*const)(),void(*)()>>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::map<void(*)(),void(*)(),std::less<void(*)()>,std::allocator<std::pair<void(*const)(),void(*)()>>>>>>;

typedef struct {
} map<std::__cxx11::string,std::pair<float,float>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::pair<float,float>>>>;

typedef struct {
} map<std::__cxx11::string,std::vector<int,std::allocator<int>>,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::vector<int,std::allocator<int>>>>>;

typedef struct {
} map<std::__cxx11::string_const,std::__cxx11::string_const,std::less<std::__cxx11::string_const>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string_const>>>;

typedef struct {
} map<unsigned_long,int,std::less<unsigned_long>,std::allocator<std::pair<unsigned_long_const,int>>>;

typedef struct {
} map<void(*)(),void(*)(),std::less<void(*)()>,std::allocator<std::pair<void(*const)(),void(*)()>>>;

typedef struct {
} MapPartitioned3d<double,int>;

typedef struct {
} MapPinName2Idx;

typedef struct {
} Mat11;

typedef struct {
} MatCol;

typedef struct {
} Matrix;

typedef struct {
} MatrixElement;

typedef struct {
} MatrixEqn;

typedef struct {
} MatrixPartition;

typedef struct {
} MatrixPartition<int>;

typedef struct {
} MatRow;

typedef struct {
} MaxAvgRms;

typedef struct {
} MDie;

typedef struct {
} Merge<SelectSparse<int>,float,float,float>;

typedef struct {
} MergeAdsmem;

typedef struct {
} MergeEnd<SelectSparse<int>,MinAveMax<float,double>,MinAveMax<float,double>,MinAveMax<float,double>>;

typedef struct {
} Message;

typedef struct {
} MessageSys;

typedef struct {
} MessageSysEntry;

typedef struct {
} mfs_options;

typedef struct {
} MfSolver;

typedef struct {
} MfsolverLib;

typedef struct {
} MinAvgWC;

typedef struct {
} MinInstDv;

typedef struct {
} MinInstDvPercent;

typedef struct {
} MmapSysAllocator;

typedef struct {
} mmxDvdGlitchFilter;

typedef struct {
} mmxPwlWave;

typedef struct {
} MT19937Engine;

typedef struct {
} MultiBitGsc;

typedef struct {
} MultiDie;

typedef struct {
} multind;

typedef struct {
} NameSorter;

typedef struct {
} NamesRead;

typedef struct {
} Network;

typedef struct {
} NetworkBuild;

typedef struct {
} NetworkCouplingPoints;

typedef struct {
} NetworkImport;

typedef struct {
} NetworkSim;

typedef struct {
} new_allocator;

typedef struct {
} new_allocator<std::_List_node<anslic_feature*>>;

typedef struct {
} new_allocator<std::_List_node<CAnsFeatureUsage>>;

typedef struct {
} new_allocator<std::_List_node<CAnsLicBulkCapRequest>>;

typedef struct {
} new_allocator<std::_List_node<CAnsLicContextFeatureData>>;

typedef struct {
} new_allocator<std::_List_node<CAnsServerInfo>>;

typedef struct {
} new_allocator<std::_List_node<int>>;

typedef struct {
} new_allocator<std::_List_node<request*>>;

typedef struct {
} new_allocator<std::_List_node<std::__cxx11::string>>;

typedef struct {
} new_allocator<std::_List_node<std::pair<int,CAnsLicLicenseData*>>>;

typedef struct {
} new_allocator<std::_List_node<std::pair<std::__cxx11::string,int>>>;

typedef struct {
} new_allocator<std::_List_node<std::pair<std::__cxx11::string,std::__cxx11::string>>>;

typedef struct {
} new_allocator<std::_Rb_tree_node<std::__cxx11::string>>;

typedef struct {
} new_allocator<std::_Rb_tree_node<std::pair<ANSLIC_MAX_SET_LIMITS_const,int>>>;

typedef struct {
} new_allocator<std::_Rb_tree_node<std::pair<int_const,anslic_feature*>>>;

typedef struct {
} new_allocator<std::_Rb_tree_node<std::pair<int_const,std::__cxx11::string>>>;

typedef struct {
} new_allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,anslic_bool>>>;

typedef struct {
} new_allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,anslic_client*>>>;

typedef struct {
} new_allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,anslic_feature*>>>;

typedef struct {
} new_allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,CAnsLicLicenseData*>>>;

typedef struct {
} new_allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,request*>>>;

typedef struct {
} new_allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,std::__cxx11::list<CAnsFeatureUsage,std::allocator<CAnsFeatureUsage>>>>>;

typedef struct {
} new_allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>;

typedef struct {
} new_allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,std::map<std::__cxx11::string,std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>>>>;

typedef struct {
} new_allocator<std::_Rb_tree_node<std::pair<std::__cxx11::string_const,std::map<void(*)(),void(*)(),std::less<void(*)()>,std::allocator<std::pair<void(*const)(),void(*)()>>>>>>;

typedef struct {
} new_allocator<std::_Rb_tree_node<std::pair<void(*const)(),void(*)()>>>;

typedef struct {
} newDvdGlitchRule;

typedef struct {
} Node;

typedef struct {
} NodeGroup;

typedef struct {
} NodeGroupVec;

typedef struct {
} NodeIdPair;

typedef struct {
} NodeInfo;

typedef struct {
} NodesIter;

typedef struct {
} NodeWorst0;

typedef struct {
} NodeWorstSave;

typedef struct {
} NodeWorstSearch;

typedef struct {
} NoiseResistor;

typedef struct {
} NonSimTime;

typedef struct {
} NormalPostProcesses;

typedef struct {
    /* +0x0 (len 4) */
    dword namesz;
    /* +0x4 (len 4) */
    dword descsz;
    /* +0x8 (len 4) */
    dword type;
    /* +0xc (len 4) */
    string name;
    /* +0x10 (len 4) */
    dword abiType;
    /* +0x14 (len 12) */
    dword[3] requiredKernelVersion;
} NoteAbiTag;

typedef struct {
} nothrow_t;

typedef struct {
} nportMdl;

typedef struct {
} nwoMTReadSameFile;

typedef struct {
} nwoReadWrite;

typedef struct {
} nwoReadWriteArrayBin<DmpPinNodeMap::NodeIdPair>;

typedef struct {
} nwoReadWriteArrayBin<int>;

typedef struct {
} nwoReadWriteArrayBin<long>;

typedef struct {
} nwoReadWriteArrayBinMT<DmpPinNodeMap::NodeIdPair>;

typedef struct {
} nwoReadWriteArrayBinMT<int>;

typedef struct {
} nwoReadWriteArrayBinMT<long>;

typedef struct {
} nwoReadWriteArrayBinMTOperation<DmpPinNodeMap::NodeIdPair>;

typedef struct {
} nwoReadWriteArrayBinMTOperation<int>;

typedef struct {
} nwoReadWriteArrayBinMTOperation<long>;

typedef struct {
} OFileDiskCheck;

typedef struct {
} ofstream;

typedef struct {
} Options;

typedef struct {
} OrderedMultiDie;

typedef struct {
} OrderedMultiDie<DFile>;

typedef struct {
} OrderedMultiDie<FileGzip2>;

typedef struct {
} OrderedMultiDie<FileNormal>;

typedef struct {
} ostream;

typedef struct {
} ostringstream;

typedef struct {
} OutputMisc;

typedef struct {
} PackageModel;

typedef struct {
} Pad;

typedef struct {
} PadInfo;

typedef struct {
} PagedVec;

typedef struct {
} PagedVec<AplCell,Cell,128u>;

typedef struct {
} PagedVec<Bias,Bias,128u>;

typedef struct {
} PagedVec<bool,bool,128u>;

typedef struct {
} PagedVec<bool,Instance,128u>;

typedef struct {
} PagedVec<bool,Node,128u>;

typedef struct {
} PagedVec<bool,Resistor,128u>;

typedef struct {
} PagedVec<CcCs,CcCs,128u>;

typedef struct {
} PagedVec<CcVs,CcVs,128u>;

typedef struct {
} PagedVec<Cell,Cell,128u>;

typedef struct {
} PagedVec<char,char,128u>;

typedef struct {
} PagedVec<CurrentModel*,Pin,128u>;

typedef struct {
} PagedVec<DCap,DCap,128u>;

typedef struct {
} PagedVec<DecapInst,DecapInst,128u>;

typedef struct {
} PagedVec<DecapPinElec,Pin,128u>;

typedef struct {
} PagedVec<Domain,Domain,128u>;

typedef struct {
} PagedVec<double,Domain,128u>;

typedef struct {
} PagedVec<double,double,128u>;

typedef struct {
} PagedVec<DvdGlitchRule,DvdGlitchRule,128u>;

typedef struct {
} PagedVec<EmElement,EmElement,128u>;

typedef struct {
} PagedVec<EmiPartition,EmiPartition,128u>;

typedef struct {
} PagedVec<EmRule,EmRule,128u>;

typedef struct {
} PagedVec<EmRuleSpec,EmRuleSpec,128u>;

typedef struct {
} PagedVec<ESDInst,ESDInst,128u>;

typedef struct {
} PagedVec<ESDModel,ESDModel,128u>;

typedef struct {
} PagedVec<float,float,128u>;

typedef struct {
} PagedVec<FlopBank,FlopBank,128u>;

typedef struct {
} PagedVec<FlopBankBitElec,FlopBankBitElec,128u>;

typedef struct {
} PagedVec<FosterVcCs,FosterVcCs,128u>;

typedef struct {
} PagedVec<Glitch,Glitch,128u>;

typedef struct {
} PagedVec<ICap,ICap,128u>;

typedef struct {
} PagedVec<Id64<InstWstEffData::InstData>,Id64<InstWstEffData::InstData>,128u>;

typedef struct {
} PagedVec<Id64<ResistorGraph::Edge>,Node,128u>;

typedef struct {
} PagedVec<Id64<ScenarioCalc::ToggleRates>,Instance,128u>;

typedef struct {
} PagedVec<Id<char>,Id<char>,128u>;

typedef struct {
} PagedVec<Id<Instance>,Id<Instance>,128u>;

typedef struct {
} PagedVec<Id<Node>,Node,128u>;

typedef struct {
} PagedVec<Id<ScenarioCalc::VlessScenario>,Instance,128u>;

typedef struct {
} PagedVec<Impedance,Impedance,128u>;

typedef struct {
} PagedVec<Inductor,Inductor,128u>;

typedef struct {
} PagedVec<InitNodeV,InitNodeV,128u>;

typedef struct {
} PagedVec<Instance,Instance,128u>;

typedef struct {
} PagedVec<InstanceElec,Instance,128u>;

typedef struct {
} PagedVec<InstLoadCap,DecapInst,128u>;

typedef struct {
} PagedVec<InstSwitching*,Instance,128u>;

typedef struct {
} PagedVec<InstSwitchingCopy*,Instance,128u>;

typedef struct {
} PagedVec<int,int,128u>;

typedef struct {
} PagedVec<IPad,IPad,128u>;

typedef struct {
} PagedVec<ISource,ISource,128u>;

typedef struct {
} PagedVec<LdoCell,LdoCell,128u>;

typedef struct {
} PagedVec<LdoInst,LdoInst,128u>;

typedef struct {
} PagedVec<LdoPin,LdoPin,128u>;

typedef struct {
} PagedVec<LdoPort,LdoPort,128u>;

typedef struct {
} PagedVec<LGroup,LGroup,128u>;

typedef struct {
} PagedVec<long,long,128u>;

typedef struct {
} PagedVec<LowPowerInst,LowPowerInst,128u>;

typedef struct {
} PagedVec<NetworkImport::ESRC4Pin,NetworkImport::ESRC4Pin,128u>;

typedef struct {
} PagedVec<newDvdGlitchRule,newDvdGlitchRule,128u>;

typedef struct {
} PagedVec<Node,Node,128u>;

typedef struct {
} PagedVec<NodeInfo,Node,128u>;

typedef struct {
} PagedVec<NoiseResistor,NoiseResistor,128u>;

typedef struct {
} PagedVec<Pad,Pad,128u>;

typedef struct {
} PagedVec<PGArc,PGArc,128u>;

typedef struct {
} PagedVec<Pin,Pin,128u>;

typedef struct {
} PagedVec<PinElec,Pin,128u>;

typedef struct {
} PagedVec<PiNode,PiNode,128u>;

typedef struct {
} PagedVec<PinStamp,PinStamp,128u>;

typedef struct {
} PagedVec<PNet,PNet,128u>;

typedef struct {
} PagedVec<PNetCalc,PNet,128u>;

typedef struct {
} PagedVec<Port,Port,128u>;

typedef struct {
} PagedVec<ProbeNode,ProbeNode,128u>;

typedef struct {
} PagedVec<PSimPad,PSimPad,128u>;

typedef struct {
} PagedVec<Pwl2DVcCs,Pwl2DVcCs,128u>;

typedef struct {
} PagedVec<PwlCap,PwlCap,128u>;

typedef struct {
} PagedVec<PwlCcVs,PwlCcVs,128u>;

typedef struct {
} PagedVec<PwlImodel,PwlImodel,128u>;

typedef struct {
} PagedVec<PwlIsrc,PwlIsrc,128u>;

typedef struct {
} PagedVec<PwlVsrc,PwlVsrc,128u>;

typedef struct {
} PagedVec<ReportDecap::RptElt,ReportDecap::RptElt,128u>;

typedef struct {
} PagedVec<ReportPartitionSupplyCurrents::EdgeICap,ReportPartitionSupplyCurrents::EdgeICap,128u>;

typedef struct {
} PagedVec<ReportPartitionSupplyCurrents::EdgeRes,ReportPartitionSupplyCurrents::EdgeRes,128u>;

typedef struct {
} PagedVec<Resistor,Resistor,128u>;

typedef struct {
} PagedVec<ResistorGraph::Edge,ResistorGraph::Edge,128u>;

typedef struct {
} PagedVec<ScenarioCalc::DomainPower,Domain,128u>;

typedef struct {
} PagedVec<ScenarioCalc::ToggleFactor,Instance,128u>;

typedef struct {
} PagedVec<ScenarioCalc::ToggleRates,ScenarioCalc::ToggleRates,128u>;

typedef struct {
} PagedVec<ScenarioCalc::VlessScenario,ScenarioCalc::VlessScenario,128u>;

typedef struct {
} PagedVec<SeriesRC,SeriesRC,128u>;

typedef struct {
} PagedVec<short,short,128u>;

typedef struct {
} PagedVec<SnapBackDevice,SnapBackDevice,128u>;

typedef struct {
} PagedVec<std::__cxx11::string,std::__cxx11::string,128u>;

typedef struct {
} PagedVec<std::pair<Id<Instance>,float>,std::pair<Id<Instance>,float>,128u>;

typedef struct {
} PagedVec<std::pair<int,float>,std::pair<int,float>,128u>;

typedef struct {
} PagedVec<SWCell,SWCell,128u>;

typedef struct {
} PagedVec<SwitchCond,Instance,128u>;

typedef struct {
} PagedVec<SwitchTimeState,SwitchTimeState,128u>;

typedef struct {
} PagedVec<Tvr,Tvr,128u>;

typedef struct {
} PagedVec<unsigned_int,unsigned_int,4u>;

typedef struct {
} PagedVec<VcCs,VcCs,128u>;

typedef struct {
} PagedVec<VcVs,VcVs,128u>;

typedef struct {
} PagedVec<Vs,Vs,128u>;

typedef struct {
} PagedVec<VSource,VSource,128u>;

typedef struct {
} PagedVecBase;

typedef struct {
} PageHeap;

typedef struct {
} Pair;

typedef struct {
} Pair;

typedef struct {
} Pair;

typedef struct {
} Pair;

typedef struct {
} Pair;

typedef struct {
} Pair;

typedef struct {
} Pair;

typedef struct {
} Pair;

typedef struct {
} pair;

typedef struct {
} pair<int,CAnsLicLicenseData*>;

typedef struct {
} pair<int,std::__cxx11::string>;

typedef struct {
} pair<int_const,anslic_feature*>;

typedef struct {
} pair<int_const,std::__cxx11::string>;

typedef struct {
} pair<std::__cxx11::string,anslic_bool>;

typedef struct {
} pair<std::__cxx11::string,anslic_client*>;

typedef struct {
} pair<std::__cxx11::string,CAnsLicLicenseData*>;

typedef struct {
} pair<std::__cxx11::string,int>;

typedef struct {
} pair<std::__cxx11::string,std::__cxx11::string>;

typedef struct {
} pair<std::__cxx11::string,std::map<std::__cxx11::string,std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>>;

typedef struct {
} pair<std::__cxx11::string_const,anslic_bool>;

typedef struct {
} pair<std::__cxx11::string_const,anslic_client*>;

typedef struct {
} pair<std::__cxx11::string_const,anslic_feature*>;

typedef struct {
} pair<std::__cxx11::string_const,CAnsLicLicenseData*>;

typedef struct {
} pair<std::__cxx11::string_const,request*>;

typedef struct {
} pair<std::__cxx11::string_const,std::__cxx11::list<CAnsFeatureUsage,std::allocator<CAnsFeatureUsage>>>;

typedef struct {
} pair<std::__cxx11::string_const,std::__cxx11::string>;

typedef struct {
} pair<std::__cxx11::string_const,std::__cxx11::string_const>;

typedef struct {
} pair<std::__cxx11::string_const,std::map<std::__cxx11::string,std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>>;

typedef struct {
} pair<std::__cxx11::string_const,std::map<void(*)(),void(*)(),std::less<void(*)()>,std::allocator<std::pair<void(*const)(),void(*)()>>>>;

typedef struct {
} pair<std::_Rb_tree_const_iterator<std::__cxx11::string>,bool>;

typedef struct {
} pair<std::_Rb_tree_iterator<std::__cxx11::string>,bool>;

typedef struct {
} pair<std::_Rb_tree_iterator<std::pair<int_const,anslic_feature*>>,bool>;

typedef struct {
} pair<std::_Rb_tree_iterator<std::pair<int_const,std::__cxx11::string>>,bool>;

typedef struct {
} pair<std::_Rb_tree_iterator<std::pair<std::__cxx11::string_const,anslic_bool>>,bool>;

typedef struct {
} pair<std::_Rb_tree_iterator<std::pair<std::__cxx11::string_const,anslic_client*>>,bool>;

typedef struct {
} pair<std::_Rb_tree_iterator<std::pair<std::__cxx11::string_const,anslic_feature*>>,bool>;

typedef struct {
} pair<std::_Rb_tree_iterator<std::pair<std::__cxx11::string_const,CAnsLicLicenseData*>>,bool>;

typedef struct {
} pair<std::_Rb_tree_iterator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>,bool>;

typedef struct {
} pair<std::_Rb_tree_iterator<std::pair<std::__cxx11::string_const,std::map<std::__cxx11::string,std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::pair<std::__cxx11::string_const,std::__cxx11::string>>>>>,bool>;

typedef struct {
} pair<std::_Rb_tree_node_base*,std::_Rb_tree_node_base*>;

typedef struct {
} PairV;

typedef struct {
    /* +0x0 (len 8) */
    char * pw_name;
    /* +0x8 (len 8) */
    char * pw_passwd;
    /* +0x10 (len 4) */
    __uid_t pw_uid;
    /* +0x14 (len 4) */
    __gid_t pw_gid;
    /* +0x18 (len 8) */
    char * pw_gecos;
    /* +0x20 (len 8) */
    char * pw_dir;
    /* +0x28 (len 8) */
    char * pw_shell;
} passwd;

typedef struct {
} PComp;

typedef struct {
} PeakTailCutor;

typedef struct {
} PeriodicInstSwitching;

typedef struct {
} PermSwitchVec;

typedef struct {
} PGArc;

typedef struct {
} PHash;

typedef struct {
} Pick<SelectDense,float>;

typedef struct {
} PiModel;

typedef struct {
} Pin;

typedef struct {
} PinData;

typedef struct {
} PinData;

typedef struct {
} PinDataInternal;

typedef struct {
} PinElec;

typedef struct {
} PinICompress;

typedef struct {
} PinIPeak;

typedef struct {
} PinISection;

typedef struct {
} PinIWrite;

typedef struct {
} PinMCycleData;

typedef struct {
} PinsIter;

typedef struct {
} PinsIter;

typedef struct {
} PinsIter;

typedef struct {
} pkgparam;

typedef struct {
} PNet;

typedef struct {
} PNetMonitor;

typedef struct {
} Point;

typedef struct {
} PoleResidueForm;

typedef struct {
} Pool;

typedef struct {
} Port;

typedef struct {
} PortsIter;

typedef struct {
} PostProcess;

typedef struct {
} PowerNameSorter;

typedef struct {
} prcurelem;

typedef struct {
} probe;

typedef struct {
} ProbeNode;

typedef struct {
} ProcMapsIterator;

typedef struct {
} ProgressReporter;

typedef struct {
} prvolnode;

typedef struct {
} PsimNodeMap;

typedef struct {
} PSimPad;

typedef union {
    char[56] __size;
    long __align;
} pthread_attr_t;

typedef union {
    _struct_16 __data;
    char[48] __size;
    longlong __align;
} pthread_cond_t;

typedef union {
    char[4] __size;
    int __align;
} pthread_condattr_t;

typedef union {
    __pthread_mutex_s __data;
    char[40] __size;
    long __align;
} pthread_mutex_t;

typedef union {
    char[4] __size;
    int __align;
} pthread_mutexattr_t;

typedef struct {
} pulseisrc;

typedef struct {
} pulsevsrc;

typedef struct {
} Pwl2DTable;

typedef struct {
} Pwl2DTable<double>;

typedef struct {
} Pwl2DVcCs;

typedef struct {
} pwl2dvccs;

typedef struct {
} PwlCap;

typedef struct {
} PwlCapCell;

typedef struct {
} PwlCapLib;

typedef struct {
} pwlcaptol;

typedef struct {
} pwlccvs;

typedef struct {
} PwlImodel;

typedef struct {
} PwlIsrc;

typedef struct {
} pwlisrc;

typedef struct {
} pwlTbl;

typedef struct {
} pwlvcvs;

typedef struct {
} PwlVsrc;

typedef struct {
} pwlvsrc;

typedef struct {
} RailEffVol;

typedef struct {
} RandomMultiDie;

typedef struct {
} RandomMultiDie<DFile>;

typedef struct {
} RandomMultiDie<FileGzip2>;

typedef struct {
} RandomMultiDie<FileNormal>;

typedef struct {
} RandomNumber;

typedef struct {
} RandomSwitching<PermSwitchVec>;

typedef struct {
} RandomSwitching<TmpSwitchVec>;

typedef struct {
} RationalApproxMatrix;

typedef struct {
} RawFile2;

typedef struct {
} RawStore;

typedef struct {
} RCurrentStats;

typedef struct {
} RCurrentStatsDMP;

typedef struct {
    /* +0x0 (len 8) */
    uchar * buffer;
    /* +0x8 (len 8) */
    ulong allocated;
    /* +0x10 (len 8) */
    ulong used;
    /* +0x18 (len 8) */
    reg_syntax_t syntax;
    /* +0x20 (len 8) */
    char * fastmap;
    /* +0x28 (len 8) */
    uchar * translate;
    /* +0x30 (len 8) */
    size_t re_nsub;
    /* +0x38 (len 1) */
    uint:1 can_be_null;
    /* +0x38 (len 1) */
    uint:2 regs_allocated;
    /* +0x38 (len 1) */
    uint:1 fastmap_accurate;
    /* +0x38 (len 1) */
    uint:1 no_sub;
    /* +0x38 (len 1) */
    uint:1 not_bol;
    /* +0x38 (len 1) */
    uint:1 not_eol;
    /* +0x38 (len 1) */
    uint:1 newline_anchor;
} re_pattern_buffer;

typedef struct {
} Read;

typedef struct {
} ReadHeader;

typedef struct {
} RecursiveConvolution;

typedef struct {
} refcount_ptr<boost::exception_detail::error_info_container>;

typedef struct {
    /* +0x0 (len 4) */
    regoff_t rm_so;
    /* +0x4 (len 4) */
    regoff_t rm_eo;
} regmatch_t;

typedef struct {
} Relocate;

typedef struct {
} ReportCurrentStats<LCurrentStats>;

typedef struct {
} ReportCurrentStats<LCurrentStatsDMP>;

typedef struct {
} ReportCurrentStats<RCurrentStats>;

typedef struct {
} ReportCurrentStats<RCurrentStatsDMP>;

typedef struct {
} ReportData;

typedef struct {
} ReportData3d;

typedef struct {
} ReportDataAccess;

typedef struct {
} ReportDataAccessMCycle;

typedef struct {
} ReportDataCreate;

typedef struct {
} ReportDataMCycle3d;

typedef struct {
} ReportDataMCycleCreate;

typedef struct {
} ReportDataMCycleDvDArcCreate;

typedef struct {
} ReportDataMCycleDvDPinCreate;

typedef struct {
} ReportDataScan;

typedef struct {
} ReportDataScenario;

typedef struct {
} ReportDataScenarioCreate;

typedef struct {
} ReportDecap;

typedef struct {
} ReportDemandCurrents;

typedef struct {
} ReportDynamicBranchWave;

typedef struct {
} ReportEmResults;

typedef struct {
} ReportFrqdCurrent;

typedef struct {
} ReportingEngine;

typedef struct {
} ReportLdoResult;

typedef struct {
} ReportMcyc;

typedef struct {
} ReportMcycArc;

typedef struct {
} ReportMCycArcVerbose;

typedef struct {
} ReportMcycArcWithTime;

typedef struct {
} ReportMCycleDvDArc;

typedef struct {
} ReportMCycleDvDPin;

typedef struct {
} ReportMcycPin;

typedef struct {
} ReportMCycPinVerbose;

typedef struct {
} ReportMinMaxVoltage;

typedef struct {
} ReportP2P;

typedef struct {
} ReportPadIV;

typedef struct {
} ReportPartitionDemandCurrents;

typedef struct {
} ReportPartitionSupplyCurrents;

typedef struct {
} ReportPkgProbeNode;

typedef struct {
} ReportProbeNode;

typedef struct {
} ReportPvnWstTr;

typedef struct {
} ReportRailVoltage;

typedef struct {
} ReportRLCurrentFast;

typedef struct {
} ReportSmpI2d;

typedef struct {
} ReportSRLCurrentFast;

typedef struct {
} ReportSupplyCurrent;

typedef struct {
} ReportTimeWindow;

typedef struct {
} ReportWstArcid;

typedef struct {
} ReportWstInst;

typedef struct {
} ReportWstInstOut;

typedef struct {
} ReportWstInstOut3d;

typedef struct {
} ReportWstInstOutBin;

typedef struct {
} ReportWstInstPgarc;

typedef struct {
} ReportWstInstPgpin;

typedef struct {
} ReportWstInstTempo;

typedef struct {
} Request;

typedef struct {
} request;

typedef struct {
} ResEmStats;

typedef struct {
} Resistor;

typedef struct {
} resistor;

typedef struct {
} ResistorGraph;

typedef struct {
} RestrictPeakTailCutor;

typedef struct {
} ResultData;

typedef struct {
} ResultData;

typedef struct {
    /* +0x0 (len 8) */
    rlim_t rlim_cur;
    /* +0x8 (len 8) */
    rlim_t rlim_max;
} rlimit;

typedef struct {
} RLNodePairReader<int>;

typedef struct {
} RLNodePairReader<long>;

typedef struct {
} RLNodePairWriter;

typedef struct {
} RptElt;

typedef struct {
} rs_mfs_uwrapper_lib;

typedef struct {
} RunData;

typedef struct {
} RunInfo;

typedef struct {
} runTime;

typedef struct {
} runtime_error;

typedef struct {
    /* +0x0 (len 16) */
    timeval ru_utime;
    /* +0x10 (len 16) */
    timeval ru_stime;
    /* +0x20 (len 8) */
    long ru_maxrss;
    /* +0x28 (len 8) */
    long ru_ixrss;
    /* +0x30 (len 8) */
    long ru_idrss;
    /* +0x38 (len 8) */
    long ru_isrss;
    /* +0x40 (len 8) */
    long ru_minflt;
    /* +0x48 (len 8) */
    long ru_majflt;
    /* +0x50 (len 8) */
    long ru_nswap;
    /* +0x58 (len 8) */
    long ru_inblock;
    /* +0x60 (len 8) */
    long ru_oublock;
    /* +0x68 (len 8) */
    long ru_msgsnd;
    /* +0x70 (len 8) */
    long ru_msgrcv;
    /* +0x78 (len 8) */
    long ru_nsignals;
    /* +0x80 (len 8) */
    long ru_nvcsw;
    /* +0x88 (len 8) */
    long ru_nivcsw;
} rusage;

typedef struct {
} Sampler;

typedef struct {
} SaveCompressedVFile;

typedef struct {
} SbrkSysAllocator;

typedef struct {
} Scanner;

typedef struct {
} ScenarioCalc;

typedef struct {
} Section;

typedef struct {
} SectionVector;

typedef struct {
} SelectDense;

typedef struct {
} SelectSparse<int>;

typedef struct {
} SeriesRC;

typedef struct {
} set;

typedef struct {
} set<int,std::less<int>,std::allocator<int>>;

typedef struct {
} set<std::__cxx11::string,std::less<std::__cxx11::string>,std::allocator<std::__cxx11::string>>;

typedef struct {
} set<std::pair<long,long>,std::less<std::pair<long,long>>,std::allocator<std::pair<long,long>>>;

typedef struct {
    /* +0x0 (len 48) */
    ipc_perm shm_perm;
    /* +0x30 (len 8) */
    size_t shm_segsz;
    /* +0x38 (len 8) */
    __time_t shm_atime;
    /* +0x40 (len 8) */
    __time_t shm_dtime;
    /* +0x48 (len 8) */
    __time_t shm_ctime;
    /* +0x50 (len 4) */
    __pid_t shm_cpid;
    /* +0x54 (len 4) */
    __pid_t shm_lpid;
    /* +0x58 (len 8) */
    shmatt_t shm_nattch;
    /* +0x60 (len 8) */
    ulong __unused4;
    /* +0x68 (len 8) */
    ulong __unused5;
} shmid_ds;

typedef struct {
    /* +0x0 (len 8) */
    _union_1457 __sigaction_handler;
    /* +0x8 (len 128) */
    __sigset_t sa_mask;
    /* +0x88 (len 4) */
    int sa_flags;
    /* +0x90 (len 8) */
    _func_5327 * sa_restorer;
} sigaction;

typedef struct {
    /* +0x0 (len 4) */
    int si_signo;
    /* +0x4 (len 4) */
    int si_errno;
    /* +0x8 (len 4) */
    int si_code;
    /* +0x10 (len 112) */
    _union_1441 _sifields;
} siginfo;

typedef union {
    int sival_int;
    void * sival_ptr;
} sigval;

typedef struct {
} simplifyPath;

typedef struct {
} SimState;

typedef struct {
} Simulation;

typedef struct {
} sinisrc;

typedef struct {
} sinvsrc;

typedef struct {
} SizeMap;

typedef struct {
} SmallArcVoltages;

typedef struct {
} SmallPinVoltages;

typedef struct {
} SmallSpanStats;

typedef struct {
} SnapBackDevice;

typedef struct {
} socket_exception;

typedef struct {
} SortOrder3d<float,ReportInstancePrune>;

typedef struct {
} SortOrder<float>;

typedef struct {
} Span;

typedef struct {
} SpanList;

typedef struct {
} sparam;

typedef struct {
} SpinLock;

typedef struct {
} stack<ElemSubMdl*,std::deque<ElemSubMdl*,std::allocator<ElemSubMdl*>>>;

typedef struct {
} stack<trietree<int*>*,std::deque<trietree<int*>*,std::allocator<trietree<int*>*>>>;

typedef struct {
} StackTrace;

typedef struct {
} StackTraceTable;

typedef struct {
} StampThread2<Impedance>;

typedef struct {
} StampThread2<PinStamp>;

typedef struct {
} StampThread2<PwlImodel>;

typedef struct {
} StampThread2<PwlIsrc>;

typedef struct {
} StampThread2<SeriesRC>;

typedef struct {
} StampThread<FosterVcCs,FosterVcCs>;

typedef struct {
} StampThread<Glitch,Glitch>;

typedef struct {
} StampThread<ICap,ICap>;

typedef struct {
} StampThread<Inductor,Inductor>;

typedef struct {
} StampThread<Instance,Instance>;

typedef struct {
} StampThread<LGroup,LGroup>;

typedef struct {
} StampThread<LowPowerInst,LowPowerInst>;

typedef struct {
} StampThread<Node,Node>;

typedef struct {
} StampThread<PSimPad,PSimPad>;

typedef struct {
} StampThread<PwlCap,PwlCap>;

typedef struct {
} StampThread<PwlCcVs,PwlCcVs>;

typedef struct {
} StampThread<PwlVsrc,PwlVsrc>;

typedef struct {
} StampThread<SnapBackDevice,SnapBackDevice>;

typedef struct {
} StampThread<Tvr,Tvr>;

typedef struct {
    /* +0x0 (len 8) */
    __dev_t st_dev;
    /* +0x8 (len 8) */
    __ino_t st_ino;
    /* +0x10 (len 8) */
    __nlink_t st_nlink;
    /* +0x18 (len 4) */
    __mode_t st_mode;
    /* +0x1c (len 4) */
    __uid_t st_uid;
    /* +0x20 (len 4) */
    __gid_t st_gid;
    /* +0x24 (len 4) */
    int __pad0;
    /* +0x28 (len 8) */
    __dev_t st_rdev;
    /* +0x30 (len 8) */
    __off_t st_size;
    /* +0x38 (len 8) */
    __blksize_t st_blksize;
    /* +0x40 (len 8) */
    __blkcnt_t st_blocks;
    /* +0x48 (len 16) */
    timespec st_atim;
    /* +0x58 (len 16) */
    timespec st_mtim;
    /* +0x68 (len 16) */
    timespec st_ctim;
    /* +0x78 (len 24) */
    long[3] __unused;
} stat;

typedef struct {
    /* +0x0 (len 8) */
    __dev_t st_dev;
    /* +0x8 (len 8) */
    __ino64_t st_ino;
    /* +0x10 (len 8) */
    __nlink_t st_nlink;
    /* +0x18 (len 4) */
    __mode_t st_mode;
    /* +0x1c (len 4) */
    __uid_t st_uid;
    /* +0x20 (len 4) */
    __gid_t st_gid;
    /* +0x24 (len 4) */
    int __pad0;
    /* +0x28 (len 8) */
    __dev_t st_rdev;
    /* +0x30 (len 8) */
    __off_t st_size;
    /* +0x38 (len 8) */
    __blksize_t st_blksize;
    /* +0x40 (len 8) */
    __blkcnt64_t st_blocks;
    /* +0x48 (len 16) */
    timespec st_atim;
    /* +0x58 (len 16) */
    timespec st_mtim;
    /* +0x68 (len 16) */
    timespec st_ctim;
    /* +0x78 (len 24) */
    long[3] __unused;
} stat64;

typedef struct {
} state_iprof;

typedef struct {
} state_timing;

typedef struct {
} StateElementVec<Impedance>;

typedef struct {
} StateElementVec<PinStamp>;

typedef struct {
} StateElementVec<PwlImodel>;

typedef struct {
} StateElementVec<PwlIsrc>;

typedef struct {
} StateElementVec<SeriesRC>;

typedef struct {
    /* +0x0 (len 8) */
    long f_type;
    /* +0x8 (len 8) */
    long f_bsize;
    /* +0x10 (len 8) */
    __fsblkcnt_t f_blocks;
    /* +0x18 (len 8) */
    __fsblkcnt_t f_bfree;
    /* +0x20 (len 8) */
    __fsblkcnt_t f_bavail;
    /* +0x28 (len 8) */
    __fsfilcnt_t f_files;
    /* +0x30 (len 8) */
    __fsfilcnt_t f_ffree;
    /* +0x38 (len 8) */
    __fsid_t f_fsid;
    /* +0x40 (len 8) */
    long f_namelen;
    /* +0x48 (len 8) */
    long f_frsize;
    /* +0x50 (len 8) */
    long f_flags;
    /* +0x58 (len 32) */
    long[4] f_spare;
} statfs;

typedef struct {
} StaticData<CurrentFile::Write*>;

typedef struct {
} StaticData<float>;

typedef struct {
    /* +0x0 (len 8) */
    ulong f_bsize;
    /* +0x8 (len 8) */
    ulong f_frsize;
    /* +0x10 (len 8) */
    __fsblkcnt_t f_blocks;
    /* +0x18 (len 8) */
    __fsblkcnt_t f_bfree;
    /* +0x20 (len 8) */
    __fsblkcnt_t f_bavail;
    /* +0x28 (len 8) */
    __fsfilcnt_t f_files;
    /* +0x30 (len 8) */
    __fsfilcnt_t f_ffree;
    /* +0x38 (len 8) */
    __fsfilcnt_t f_favail;
    /* +0x40 (len 8) */
    ulong f_fsid;
    /* +0x48 (len 8) */
    ulong f_flag;
    /* +0x50 (len 8) */
    ulong f_namemax;
    /* +0x58 (len 24) */
    int[6] __f_spare;
} statvfs;

typedef struct {
} StopWatch;

typedef struct {
} Str;

typedef struct {
} streambuf;

typedef struct {
} string;

typedef struct {
} string;

typedef struct {
} stringbuf;

typedef struct {
} stringstream;

typedef struct {
} SumEventsCustomRandomSwitching<TmpSwitchVec>;

typedef struct {
} SWCell;

typedef struct {
} swcell;

typedef struct {
} swControlSlew;

typedef struct {
} SwitchEvent;

typedef struct {
} SwitchScenario;

typedef struct {
} SwitchState;

typedef struct {
} SwitchTimeState;

typedef struct {
} Symmetric;

typedef struct {
} Symmetric<double,int>;

typedef struct {
} SysAllocator;

typedef struct {
    /* +0x0 (len 8) */
    long uptime;
    /* +0x8 (len 24) */
    ulong[3] loads;
    /* +0x20 (len 8) */
    ulong totalram;
    /* +0x28 (len 8) */
    ulong freeram;
    /* +0x30 (len 8) */
    ulong sharedram;
    /* +0x38 (len 8) */
    ulong bufferram;
    /* +0x40 (len 8) */
    ulong totalswap;
    /* +0x48 (len 8) */
    ulong freeswap;
    /* +0x50 (len 2) */
    ushort procs;
    /* +0x52 (len 2) */
    ushort pad;
    /* +0x58 (len 8) */
    ulong totalhigh;
    /* +0x60 (len 8) */
    ulong freehigh;
    /* +0x68 (len 4) */
    uint mem_unit;
    /* +0x6c (len 0) */
    char[0] _f;
} sysinfo;

typedef struct {
} SysRandEngine;

typedef struct {
} TAG_INFO;

typedef struct {
} TCMalloc_Printer;

typedef struct {
} TCMallocGuard;

typedef struct {
} TCMallocImplementation;

typedef struct {
} TempInst;

typedef struct {
} TempTvr;

typedef struct {
    /* +0x0 (len 4) */
    tcflag_t c_iflag;
    /* +0x4 (len 4) */
    tcflag_t c_oflag;
    /* +0x8 (len 4) */
    tcflag_t c_cflag;
    /* +0xc (len 4) */
    tcflag_t c_lflag;
    /* +0x10 (len 1) */
    cc_t c_line;
    /* +0x11 (len 32) */
    cc_t[32] c_cc;
    /* +0x34 (len 4) */
    speed_t c_ispeed;
    /* +0x38 (len 4) */
    speed_t c_ospeed;
} termios;

typedef struct {
} Thread;

typedef struct {
} ThreadCache;

typedef struct {
} ThreadCode;

typedef struct {
} Timer;

typedef struct {
    /* +0x0 (len 8) */
    __time_t tv_sec;
    /* +0x8 (len 8) */
    long tv_nsec;
} timespec;

typedef struct {
} TimeTailCutor;

typedef struct {
    /* +0x0 (len 8) */
    __time_t tv_sec;
    /* +0x8 (len 8) */
    __suseconds_t tv_usec;
} timeval;

typedef struct {
    /* +0x0 (len 4) */
    int tz_minuteswest;
    /* +0x4 (len 4) */
    int tz_dsttime;
} timezone;

typedef struct {
    /* +0x0 (len 4) */
    int tm_sec;
    /* +0x4 (len 4) */
    int tm_min;
    /* +0x8 (len 4) */
    int tm_hour;
    /* +0xc (len 4) */
    int tm_mday;
    /* +0x10 (len 4) */
    int tm_mon;
    /* +0x14 (len 4) */
    int tm_year;
    /* +0x18 (len 4) */
    int tm_wday;
    /* +0x1c (len 4) */
    int tm_yday;
    /* +0x20 (len 4) */
    int tm_isdst;
    /* +0x28 (len 8) */
    long tm_gmtoff;
    /* +0x30 (len 8) */
    char * tm_zone;
} tm;

typedef struct {
} TmpSwitchVec;

typedef struct {
    /* +0x0 (len 8) */
    clock_t tms_utime;
    /* +0x8 (len 8) */
    clock_t tms_stime;
    /* +0x10 (len 8) */
    clock_t tms_cutime;
    /* +0x18 (len 8) */
    clock_t tms_cstime;
} tms;

typedef struct {
} ToggleFactor;

typedef struct {
} ToggleRates;

typedef struct {
} ToggleSwitching;

typedef struct {
} trienode<inductor*>;

typedef struct {
} trienode<int*>;

typedef struct {
} trienode<pulsevsrc*>;

typedef struct {
} trienode<pwlvsrc*>;

typedef struct {
} trienode<resistor*>;

typedef struct {
} trienode<sinvsrc*>;

typedef struct {
} trienode<vsource*>;

typedef struct {
} trietree<inductor*>;

typedef struct {
} trietree<int*>;

typedef struct {
} trietree<pulsevsrc*>;

typedef struct {
} trietree<pwlvsrc*>;

typedef struct {
} trietree<resistor*>;

typedef struct {
} trietree<sinvsrc*>;

typedef struct {
} trietree<vsource*>;

typedef struct {
} TsvNodeCurrent;

typedef struct {
} TT800Engine;

typedef struct {
} Tvr;

typedef struct {
} tvr;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type;

typedef struct {
} type_info;

typedef struct {
    /* +0x0 (len 8) */
    __time_t actime;
    /* +0x8 (len 8) */
    __time_t modtime;
} utimbuf;

typedef struct {
    /* +0x0 (len 65) */
    char[65] sysname;
    /* +0x41 (len 65) */
    char[65] nodename;
    /* +0x82 (len 65) */
    char[65] release;
    /* +0xc3 (len 65) */
    char[65] version;
    /* +0x104 (len 65) */
    char[65] machine;
    /* +0x145 (len 65) */
    char[65] domainname;
} utsname;

typedef struct {
} Value<bool>;

typedef struct {
} Value<double>;

typedef struct {
} Value<float>;

typedef struct {
} Value<int>;

typedef struct {
} Value<long>;

typedef struct {
} Value<std::__cxx11::string>;

typedef struct {
} Value<std::vector<float,std::allocator<float>>>;

typedef struct {
} Value<std::vector<int,std::allocator<int>>>;

typedef struct {
} Value<unsigned_int>;

typedef struct {
} vccs;

typedef struct {
} VcdAdaptiveTW;

typedef struct {
} VcdCycleCompute;

typedef struct {
} vcvs;

typedef struct {
} vddEntity;

typedef struct {
} vddnode;

typedef struct {
} vddSet;

typedef struct {
} VddWPartitionFile<float,200>;

typedef struct {
} VecMax<float,float>;

typedef struct {
} VecMin<float,float>;

typedef struct {
} VecMinAveMax<float,float,double>;

typedef struct {
} VecMinOverall<float>;

typedef struct {
} VecRemapEnd<MinAveMax<float,double>,int>;

typedef struct {
} vector;

typedef struct {
} vector<_IO_FILE*,std::allocator<_IO_FILE*>>;

typedef struct {
} vector<aplLayer,std::allocator<aplLayer>>;

typedef struct {
} vector<aplLayerInfo,std::allocator<aplLayerInfo>>;

typedef struct {
} vector<aplMultiToggle*,std::allocator<aplMultiToggle*>>;

typedef struct {
} vector<ApproxResult,std::allocator<ApproxResult>>;

typedef struct {
} vector<cElementCorrector,std::allocator<cElementCorrector>>;

typedef struct {
} vector<char*,std::allocator<char*>>;

typedef struct {
} vector<char,std::allocator<char>>;

typedef struct {
} vector<char_const*,std::allocator<char_const*>>;

typedef struct {
} vector<cPassivityViolation,std::allocator<cPassivityViolation>>;

typedef struct {
} vector<cResidueRange,std::allocator<cResidueRange>>;

typedef struct {
} vector<CurrentFile::IFile*,std::allocator<CurrentFile::IFile*>>;

typedef struct {
} vector<CurrentFile::InstData,std::allocator<CurrentFile::InstData>>;

typedef struct {
} vector<CurrentFile::PinData,std::allocator<CurrentFile::PinData>>;

typedef struct {
} vector<CurrentFile::Read::CrossSectMerge*,std::allocator<CurrentFile::Read::CrossSectMerge*>>;

typedef struct {
} vector<CurrentFile::Section,std::allocator<CurrentFile::Section>>;

typedef struct {
} vector<CustomSwitchState,std::allocator<CustomSwitchState>>;

typedef struct {
} vector<double,aligned_allocator<double>>;

typedef struct {
} vector<double,std::allocator<double>>;

typedef struct {
} vector<DvdGlitchFilter::DvdGlitchTracker,std::allocator<DvdGlitchFilter::DvdGlitchTracker>>;

typedef struct {
} vector<DvdGlitchFilter::Selector<DvdGlitchFilter::SelInt<int>>::Iterator,std::allocator<DvdGlitchFilter::Selector<DvdGlitchFilter::SelInt<int>>::Iterator>>;

typedef struct {
} vector<DvdGlitchFilter::SelInt<int>,std::allocator<DvdGlitchFilter::SelInt<int>>>;

typedef struct {
} vector<DynamicSwitchReports::TvrIV,std::allocator<DynamicSwitchReports::TvrIV>>;

typedef struct {
} vector<Elem*,std::allocator<Elem*>>;

typedef struct {
} vector<ElemSub*,std::allocator<ElemSub*>>;

typedef struct {
} vector<ElemSubMdl*,std::allocator<ElemSubMdl*>>;

typedef struct {
} vector<EsdDiodeIpeak::probe,std::allocator<EsdDiodeIpeak::probe>>;

typedef struct {
} vector<EsdStressCheck::stress_data,std::allocator<EsdStressCheck::stress_data>>;

typedef struct {
} vector<float,std::allocator<float>>;

typedef struct {
} vector<FlopBankBitElec,std::allocator<FlopBankBitElec>>;

typedef struct {
} vector<FlopBankRail,std::allocator<FlopBankRail>>;

typedef struct {
} vector<FortranComplexMatrix,std::allocator<FortranComplexMatrix>>;

typedef struct {
} vector<FortranDoubleMatrix*,std::allocator<FortranDoubleMatrix*>>;

typedef struct {
} vector<GenerateInstPowerModel::Element,std::allocator<GenerateInstPowerModel::Element>>;

typedef struct {
} vector<GenerateInstPowerModel::WaveElem<long>,std::allocator<GenerateInstPowerModel::WaveElem<long>>>;

typedef struct {
} vector<Id64<char>,std::allocator<Id64<char>>>;

typedef struct {
} vector<Id<Bias>,std::allocator<Id<Bias>>>;

typedef struct {
} vector<Id<Impedance>,std::allocator<Id<Impedance>>>;

typedef struct {
} vector<Id<Instance>,std::allocator<Id<Instance>>>;

typedef struct {
} vector<Id<Node>,std::allocator<Id<Node>>>;

typedef struct {
} vector<Id<Tvr>,std::allocator<Id<Tvr>>>;

typedef struct {
} vector<IdWt,std::allocator<IdWt>>;

typedef struct {
} vector<Impedance::SimState,std::allocator<Impedance::SimState>>;

typedef struct {
} vector<IndexSortval,std::allocator<IndexSortval>>;

typedef struct {
} vector<InstPowerModel::RefNode,std::allocator<InstPowerModel::RefNode>>;

typedef struct {
} vector<InstQ01,std::allocator<InstQ01>>;

typedef struct {
} vector<int,aligned_allocator<int>>;

typedef struct {
} vector<int,std::allocator<int>>;

typedef struct {
} vector<long,std::allocator<long>>;

typedef struct {
} vector<MallocExtension::FreeListInfo,std::allocator<MallocExtension::FreeListInfo>>;

typedef struct {
} vector<MatrixElement<double,int>,std::allocator<MatrixElement<double,int>>>;

typedef struct {
} vector<MaxAvgRms,std::allocator<MaxAvgRms>>;

typedef struct {
} vector<mmxDvdGlitchFilter::GlitchResult,std::allocator<mmxDvdGlitchFilter::GlitchResult>>;

typedef struct {
} vector<MmxV,std::allocator<MmxV>>;

typedef struct {
} vector<NetworkBuild::CellStatus,std::allocator<NetworkBuild::CellStatus>>;

typedef struct {
} vector<NetworkBuild::TempTvrIndex,std::allocator<NetworkBuild::TempTvrIndex>>;

typedef struct {
} vector<NetworkImport::Pin4Inst,std::allocator<NetworkImport::Pin4Inst>>;

typedef struct {
} vector<NetworkImport::PNetMonitor,std::allocator<NetworkImport::PNetMonitor>>;

typedef struct {
} vector<NodeGroup,std::allocator<NodeGroup>>;

typedef struct {
} vector<NvbSelectNode,std::allocator<NvbSelectNode>>;

typedef struct {
} vector<Pin,std::allocator<Pin>>;

typedef struct {
} vector<PinElec,std::allocator<PinElec>>;

typedef struct {
} vector<PinStamp::SimState,std::allocator<PinStamp::SimState>>;

typedef struct {
} vector<Point,std::allocator<Point>>;

typedef struct {
} vector<PoleResidueForm,std::allocator<PoleResidueForm>>;

typedef struct {
} vector<prcurelem*,std::allocator<prcurelem*>>;

typedef struct {
} vector<Pwl2DTable<double>*,std::allocator<Pwl2DTable<double>*>>;

typedef struct {
} vector<PwlImodel::SimState,std::allocator<PwlImodel::SimState>>;

typedef struct {
} vector<PwlIsrc::SimState,std::allocator<PwlIsrc::SimState>>;

typedef struct {
} vector<RailEffVol::PairV<float>,std::allocator<RailEffVol::PairV<float>>>;

typedef struct {
} vector<RailEffVol::PairV<int>,std::allocator<RailEffVol::PairV<int>>>;

typedef struct {
} vector<RationalApproxMatrix,std::allocator<RationalApproxMatrix>>;

typedef struct {
} vector<RawStore<AplCell>*,std::allocator<RawStore<AplCell>*>>;

typedef struct {
} vector<RawStore<Bias>*,std::allocator<RawStore<Bias>*>>;

typedef struct {
} vector<RawStore<CcCs>*,std::allocator<RawStore<CcCs>*>>;

typedef struct {
} vector<RawStore<CcVs>*,std::allocator<RawStore<CcVs>*>>;

typedef struct {
} vector<RawStore<Cell>*,std::allocator<RawStore<Cell>*>>;

typedef struct {
} vector<RawStore<char>*,std::allocator<RawStore<char>*>>;

typedef struct {
} vector<RawStore<DCap>*,std::allocator<RawStore<DCap>*>>;

typedef struct {
} vector<RawStore<DecapInst>*,std::allocator<RawStore<DecapInst>*>>;

typedef struct {
} vector<RawStore<DecapPinElec>*,std::allocator<RawStore<DecapPinElec>*>>;

typedef struct {
} vector<RawStore<Domain>*,std::allocator<RawStore<Domain>*>>;

typedef struct {
} vector<RawStore<DvdGlitchRule>*,std::allocator<RawStore<DvdGlitchRule>*>>;

typedef struct {
} vector<RawStore<EmElement>*,std::allocator<RawStore<EmElement>*>>;

typedef struct {
} vector<RawStore<EmiPartition>*,std::allocator<RawStore<EmiPartition>*>>;

typedef struct {
} vector<RawStore<EmRule>*,std::allocator<RawStore<EmRule>*>>;

typedef struct {
} vector<RawStore<EmRuleSpec>*,std::allocator<RawStore<EmRuleSpec>*>>;

typedef struct {
} vector<RawStore<ESDInst>*,std::allocator<RawStore<ESDInst>*>>;

typedef struct {
} vector<RawStore<ESDModel>*,std::allocator<RawStore<ESDModel>*>>;

typedef struct {
} vector<RawStore<float>*,std::allocator<RawStore<float>*>>;

typedef struct {
} vector<RawStore<FlopBank>*,std::allocator<RawStore<FlopBank>*>>;

typedef struct {
} vector<RawStore<FlopBankBitElec>*,std::allocator<RawStore<FlopBankBitElec>*>>;

typedef struct {
} vector<RawStore<FosterVcCs>*,std::allocator<RawStore<FosterVcCs>*>>;

typedef struct {
} vector<RawStore<Glitch>*,std::allocator<RawStore<Glitch>*>>;

typedef struct {
} vector<RawStore<ICap>*,std::allocator<RawStore<ICap>*>>;

typedef struct {
} vector<RawStore<Id64<InstWstEffData::InstData>>*,std::allocator<RawStore<Id64<InstWstEffData::InstData>>*>>;

typedef struct {
} vector<RawStore<Id64<ResistorGraph::Edge>>*,std::allocator<RawStore<Id64<ResistorGraph::Edge>>*>>;

typedef struct {
} vector<RawStore<Id<char>>*,std::allocator<RawStore<Id<char>>*>>;

typedef struct {
} vector<RawStore<Id<Instance>>*,std::allocator<RawStore<Id<Instance>>*>>;

typedef struct {
} vector<RawStore<Id<Node>>*,std::allocator<RawStore<Id<Node>>*>>;

typedef struct {
} vector<RawStore<Id<ScenarioCalc::VlessScenario>>*,std::allocator<RawStore<Id<ScenarioCalc::VlessScenario>>*>>;

typedef struct {
} vector<RawStore<Impedance>*,std::allocator<RawStore<Impedance>*>>;

typedef struct {
} vector<RawStore<Inductor>*,std::allocator<RawStore<Inductor>*>>;

typedef struct {
} vector<RawStore<InitNodeV>*,std::allocator<RawStore<InitNodeV>*>>;

typedef struct {
} vector<RawStore<Instance>*,std::allocator<RawStore<Instance>*>>;

typedef struct {
} vector<RawStore<InstanceElec>*,std::allocator<RawStore<InstanceElec>*>>;

typedef struct {
} vector<RawStore<InstLoadCap>*,std::allocator<RawStore<InstLoadCap>*>>;

typedef struct {
} vector<RawStore<InstWstEffData::InstData>*,std::allocator<RawStore<InstWstEffData::InstData>*>>;

typedef struct {
} vector<RawStore<int>*,std::allocator<RawStore<int>*>>;

typedef struct {
} vector<RawStore<IPad>*,std::allocator<RawStore<IPad>*>>;

typedef struct {
} vector<RawStore<ISource>*,std::allocator<RawStore<ISource>*>>;

typedef struct {
} vector<RawStore<LdoCell>*,std::allocator<RawStore<LdoCell>*>>;

typedef struct {
} vector<RawStore<LdoInst>*,std::allocator<RawStore<LdoInst>*>>;

typedef struct {
} vector<RawStore<LdoPin>*,std::allocator<RawStore<LdoPin>*>>;

typedef struct {
} vector<RawStore<LdoPort>*,std::allocator<RawStore<LdoPort>*>>;

typedef struct {
} vector<RawStore<LGroup>*,std::allocator<RawStore<LGroup>*>>;

typedef struct {
} vector<RawStore<long>*,std::allocator<RawStore<long>*>>;

typedef struct {
} vector<RawStore<LowPowerInst>*,std::allocator<RawStore<LowPowerInst>*>>;

typedef struct {
} vector<RawStore<NetworkImport::ESRC4Pin>*,std::allocator<RawStore<NetworkImport::ESRC4Pin>*>>;

typedef struct {
} vector<RawStore<newDvdGlitchRule>*,std::allocator<RawStore<newDvdGlitchRule>*>>;

typedef struct {
} vector<RawStore<Node>*,std::allocator<RawStore<Node>*>>;

typedef struct {
} vector<RawStore<NodeInfo>*,std::allocator<RawStore<NodeInfo>*>>;

typedef struct {
} vector<RawStore<NoiseResistor>*,std::allocator<RawStore<NoiseResistor>*>>;

typedef struct {
} vector<RawStore<Pad>*,std::allocator<RawStore<Pad>*>>;

typedef struct {
} vector<RawStore<PGArc>*,std::allocator<RawStore<PGArc>*>>;

typedef struct {
} vector<RawStore<Pin>*,std::allocator<RawStore<Pin>*>>;

typedef struct {
} vector<RawStore<PinElec>*,std::allocator<RawStore<PinElec>*>>;

typedef struct {
} vector<RawStore<PiNode>*,std::allocator<RawStore<PiNode>*>>;

typedef struct {
} vector<RawStore<PinStamp>*,std::allocator<RawStore<PinStamp>*>>;

typedef struct {
} vector<RawStore<PNet>*,std::allocator<RawStore<PNet>*>>;

typedef struct {
} vector<RawStore<Port>*,std::allocator<RawStore<Port>*>>;

typedef struct {
} vector<RawStore<ProbeNode>*,std::allocator<RawStore<ProbeNode>*>>;

typedef struct {
} vector<RawStore<PSimPad>*,std::allocator<RawStore<PSimPad>*>>;

typedef struct {
} vector<RawStore<Pwl2DVcCs>*,std::allocator<RawStore<Pwl2DVcCs>*>>;

typedef struct {
} vector<RawStore<PwlCap>*,std::allocator<RawStore<PwlCap>*>>;

typedef struct {
} vector<RawStore<PwlCcVs>*,std::allocator<RawStore<PwlCcVs>*>>;

typedef struct {
} vector<RawStore<PwlImodel>*,std::allocator<RawStore<PwlImodel>*>>;

typedef struct {
} vector<RawStore<PwlIsrc>*,std::allocator<RawStore<PwlIsrc>*>>;

typedef struct {
} vector<RawStore<PwlVsrc>*,std::allocator<RawStore<PwlVsrc>*>>;

typedef struct {
} vector<RawStore<ReportDecap::RptElt>*,std::allocator<RawStore<ReportDecap::RptElt>*>>;

typedef struct {
} vector<RawStore<ReportPartitionSupplyCurrents::EdgeICap>*,std::allocator<RawStore<ReportPartitionSupplyCurrents::EdgeICap>*>>;

typedef struct {
} vector<RawStore<ReportPartitionSupplyCurrents::EdgeRes>*,std::allocator<RawStore<ReportPartitionSupplyCurrents::EdgeRes>*>>;

typedef struct {
} vector<RawStore<Resistor>*,std::allocator<RawStore<Resistor>*>>;

typedef struct {
} vector<RawStore<ResistorGraph::Edge>*,std::allocator<RawStore<ResistorGraph::Edge>*>>;

typedef struct {
} vector<RawStore<ScenarioCalc::DomainPower>*,std::allocator<RawStore<ScenarioCalc::DomainPower>*>>;

typedef struct {
} vector<RawStore<ScenarioCalc::ToggleFactor>*,std::allocator<RawStore<ScenarioCalc::ToggleFactor>*>>;

typedef struct {
} vector<RawStore<ScenarioCalc::ToggleRates>*,std::allocator<RawStore<ScenarioCalc::ToggleRates>*>>;

typedef struct {
} vector<RawStore<ScenarioCalc::VlessScenario>*,std::allocator<RawStore<ScenarioCalc::VlessScenario>*>>;

typedef struct {
} vector<RawStore<SeriesRC>*,std::allocator<RawStore<SeriesRC>*>>;

typedef struct {
} vector<RawStore<short>*,std::allocator<RawStore<short>*>>;

typedef struct {
} vector<RawStore<SnapBackDevice>*,std::allocator<RawStore<SnapBackDevice>*>>;

typedef struct {
} vector<RawStore<std::__cxx11::string>*,std::allocator<RawStore<std::__cxx11::string>*>>;

typedef struct {
} vector<RawStore<std::pair<Id<Instance>,float>>*,std::allocator<RawStore<std::pair<Id<Instance>,float>>*>>;

typedef struct {
} vector<RawStore<SWCell>*,std::allocator<RawStore<SWCell>*>>;

typedef struct {
} vector<RawStore<SwitchCond>*,std::allocator<RawStore<SwitchCond>*>>;

typedef struct {
} vector<RawStore<SwitchTimeState>*,std::allocator<RawStore<SwitchTimeState>*>>;

typedef struct {
} vector<RawStore<Tvr>*,std::allocator<RawStore<Tvr>*>>;

typedef struct {
} vector<RawStore<unsigned_int>*,std::allocator<RawStore<unsigned_int>*>>;

typedef struct {
} vector<RawStore<VcCs>*,std::allocator<RawStore<VcCs>*>>;

typedef struct {
} vector<RawStore<VcVs>*,std::allocator<RawStore<VcVs>*>>;

typedef struct {
} vector<RawStore<Vs>*,std::allocator<RawStore<Vs>*>>;

typedef struct {
} vector<RawStore<VSource>*,std::allocator<RawStore<VSource>*>>;

typedef struct {
} vector<ReportData*,std::allocator<ReportData*>>;

typedef struct {
} vector<ReportData::MDie::DieInfo,std::allocator<ReportData::MDie::DieInfo>>;

typedef struct {
} vector<ReportDataAccess::ImapFile,std::allocator<ReportDataAccess::ImapFile>>;

typedef struct {
} vector<ReportMinMaxVoltage::MinMaxV,std::allocator<ReportMinMaxVoltage::MinMaxV>>;

typedef struct {
} vector<ReportPadIV::PadInfo,std::allocator<ReportPadIV::PadInfo>>;

typedef struct {
} vector<ReportSupplyCurrent::VInfo,std::allocator<ReportSupplyCurrent::VInfo>>;

typedef struct {
} vector<SectionVector<Id<Tvr>,int>*,std::allocator<SectionVector<Id<Tvr>,int>*>>;

typedef struct {
} vector<SelectDense::Iterator,std::allocator<SelectDense::Iterator>>;

typedef struct {
} vector<SelectSparse<int>::Iterator,std::allocator<SelectSparse<int>::Iterator>>;

typedef struct {
} vector<SeriesRC::SimState,std::allocator<SeriesRC::SimState>>;

typedef struct {
} vector<short,std::allocator<short>>;

typedef struct {
} vector<std::__cxx11::string,std::allocator<std::__cxx11::string>>;

typedef struct {
} vector<std::complex<double>,aligned_allocator<std::complex<double>>>;

typedef struct {
} vector<std::complex<double>,std::allocator<std::complex<double>>>;

typedef struct {
} vector<std::deque<int,std::allocator<int>>,std::allocator<std::deque<int,std::allocator<int>>>>;

typedef struct {
} vector<std::pair<double,double>,std::allocator<std::pair<double,double>>>;

typedef struct {
} vector<std::pair<float,float>,std::allocator<std::pair<float,float>>>;

typedef struct {
} vector<std::pair<Id<Node>,Id<Node>>,std::allocator<std::pair<Id<Node>,Id<Node>>>>;

typedef struct {
} vector<std::pair<int,char>,std::allocator<std::pair<int,char>>>;

typedef struct {
} vector<std::pair<int,int>,std::allocator<std::pair<int,int>>>;

typedef struct {
} vector<std::pair<int,std::__cxx11::string>,std::allocator<std::pair<int,std::__cxx11::string>>>;

typedef struct {
} vector<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>,std::allocator<std::pair<int,std::vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>>>>;

typedef struct {
} vector<std::pair<long,float>,std::allocator<std::pair<long,float>>>;

typedef struct {
} vector<std::pair<long,long>,std::allocator<std::pair<long,long>>>;

typedef struct {
} vector<std::pair<std::__cxx11::string,int>,std::allocator<std::pair<std::__cxx11::string,int>>>;

typedef struct {
} vector<std::vector<aplMultiToggle*,std::allocator<aplMultiToggle*>>,std::allocator<std::vector<aplMultiToggle*,std::allocator<aplMultiToggle*>>>>;

typedef struct {
} vector<std::vector<double,aligned_allocator<double>>,std::allocator<std::vector<double,aligned_allocator<double>>>>;

typedef struct {
} vector<std::vector<double,std::allocator<double>>*,std::allocator<std::vector<double,std::allocator<double>>*>>;

typedef struct {
} vector<std::vector<double,std::allocator<double>>,std::allocator<std::vector<double,std::allocator<double>>>>;

typedef struct {
} vector<std::vector<GenerateInstPowerModel::WaveElem<long>,std::allocator<GenerateInstPowerModel::WaveElem<long>>>,std::allocator<std::vector<GenerateInstPowerModel::WaveElem<long>,std::allocator<GenerateInstPowerModel::WaveElem<long>>>>>;

typedef struct {
} vector<std::vector<Id<Instance>,std::allocator<Id<Instance>>>,std::allocator<std::vector<Id<Instance>,std::allocator<Id<Instance>>>>>;

typedef struct {
} vector<std::vector<int,std::allocator<int>>,std::allocator<std::vector<int,std::allocator<int>>>>;

typedef struct {
} vector<std::vector<PoleResidueForm,std::allocator<PoleResidueForm>>,std::allocator<std::vector<PoleResidueForm,std::allocator<PoleResidueForm>>>>;

typedef struct {
} vector<std::vector<std::complex<double>,std::allocator<std::complex<double>>>,std::allocator<std::vector<std::complex<double>,std::allocator<std::complex<double>>>>>;

typedef struct {
} vector<std::vector<std::vector<aplMultiToggle*,std::allocator<aplMultiToggle*>>,std::allocator<std::vector<aplMultiToggle*,std::allocator<aplMultiToggle*>>>>,std::allocator<std::vector<std::vector<aplMultiToggle*,std::allocator<aplMultiToggle*>>,std::allocator<std::vector<aplMultiToggle*,std::allocator<aplMultiToggle*>>>>>>;

typedef struct {
} vector<std::vector<std::vector<Id<Instance>,std::allocator<Id<Instance>>>,std::allocator<std::vector<Id<Instance>,std::allocator<Id<Instance>>>>>,std::allocator<std::vector<std::vector<Id<Instance>,std::allocator<Id<Instance>>>,std::allocator<std::vector<Id<Instance>,std::allocator<Id<Instance>>>>>>>;

typedef struct {
} vector<SwitchTimeState,std::allocator<SwitchTimeState>>;

typedef struct {
} vector<unsigned_int,std::allocator<unsigned_int>>;

typedef struct {
} vector<unsigned_long,std::allocator<unsigned_long>>;

typedef struct {
} vector<unsigned_short,std::allocator<unsigned_short>>;

typedef struct {
} vector<vddEntity,std::allocator<vddEntity>>;

typedef struct {
} vector<vddSet,std::allocator<vddSet>>;

typedef struct {
} vector<void*,std::allocator<void*>>;

typedef struct {
} vector<Window,std::allocator<Window>>;

typedef struct {
} vector<XGraphWaveform,std::allocator<XGraphWaveform>>;

typedef struct {
} VInfo;

typedef struct {
} VirtualDomainLeakageReports;

typedef struct {
} VirtualDomainReports;

typedef struct {
} VoltageChecker;

typedef struct {
} Vs;

typedef struct {
} vsource;

typedef struct {
} wave;

typedef struct {
} WaveElem;

typedef struct {
} waveform;

typedef struct {
} WaveForm2;

typedef struct {
} WaveValue;

typedef struct {
} WinArcAve;

typedef struct {
} Window;

typedef struct {
} WindowAverage<float,double>;

typedef struct {
} WindowMinMax<float,float>;

typedef struct {
} WindowPinAverage<float,float>;

typedef struct {
} WindowRms;

typedef struct {
} WinPinAve;

typedef struct {
} WinPinMinMax;

typedef struct {
} wrapexcept<boost::bad_function_call>;

typedef struct {
} wrapexcept<boost::bad_lexical_cast>;

typedef struct {
} Write;

typedef struct {
} WriteHeader;

typedef struct {
} XGraphWaveform;

typedef struct {
} XMLNode;

typedef struct {
} XMLResults;

#endif
