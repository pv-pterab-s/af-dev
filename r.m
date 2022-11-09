[Y, M, D, H, MI, S] = datevec(now);
disp(['!! RUN ' num2str(S)])

if system('/bin/bash build.sh') ~= 0
  disp('FAIL')
  return
endif

## fn = '/home/gpryor/dev/out/run-output';
fn = '/home/gpryor/arrayire/build/run-output';
disp(fn);
fp = fopen(fn);   # for now there is only one array
dims = fread(fp, 4, 'int64');
strides = fread(fp, 4, 'int64');
offset = fread(fp, 1, 'int64');
out = fread(fp, prod(dims), 'single');
out = int32(reshape(out, dims(1), dims(2), dims(3), dims(4)));
fclose(fp);

out
find(out ~= 0)
