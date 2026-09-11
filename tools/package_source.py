"""Create a source-only distribution from an explicit directory/suffix allowlist."""
from pathlib import Path
import tarfile, hashlib
root=Path(__file__).resolve().parents[1]
out=root/'dist';out.mkdir(exist_ok=True)
files=[root/'Makefile',root/'README.md']
for directory in ('src','include','test','examples','tools','docs'):
    files.extend(p for p in (root/directory).rglob('*')
                 if p.is_file() and not p.is_symlink()
                 and p.suffix in ('.c','.h','.inc','.py','.md','.tsv','.txt'))
archive=out/'vsdlss-source.tar.gz'
with tarfile.open(archive,'w:gz') as tar:
    for p in sorted(set(files)):
        tar.add(p,arcname='vsdlss-source/'+p.relative_to(root).as_posix(),recursive=False)
checksum=hashlib.sha256(archive.read_bytes()).hexdigest()
(out/'vsdlss-source.tar.gz.sha256').write_text(checksum+'  vsdlss-source.tar.gz\n')
print(archive)
