## the following works to load data
## fn = '/home/gpryor/arrayfire/build/doo'
## fp = fopen(fn);
######### array 1
## dims = fread(fp, 4, 'int64')
## strides = fread(fp, 4, 'int64')
## offset = fread(fp, 1, 'int64')
## out = fread(fp, prod(dims), 'single');
## out = reshape(out, dims(1), dims(2), dims(3), dims(4));
######### array 2
## dims = fread(fp, 4, 'int64')
## strides = fread(fp, 4, 'int64')
## offset = fread(fp, 1, 'int64')
## out = fread(fp, prod(dims), 'single');
## fclose(fp);

# once this is there, there should be an output file...
system('/bin/bash build.sh')

fn = 'out/doo';
fp = fopen(fn);
dims = fread(fp, 4, 'int64');
strides = fread(fp, 4, 'int64');
offset = fread(fp, 1, 'int64');
out = fread(fp, prod(dims), 'int');
out = reshape(out, dims(1), dims(2), dims(3), dims(4));
fclose(fp);

out(1:2:end,1:2:end)
