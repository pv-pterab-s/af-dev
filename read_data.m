## -*- compile-command: "octave test_read_data.m"; -*-
function [A] = read_data(fn)
  fp = fopen(fn);
  fread(fp, 4, 'int64')
  fclose(fp);
  ## A = rand(3,3);
endfunction
