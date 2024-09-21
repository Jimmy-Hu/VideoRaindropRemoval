function out = DCT3D(x)
    [N1,N2,N3] = size(x);
    out = DCT1D(x, 1);
    out = DCT1D(out, 2);
    out = DCT1D(out, 3);
    out = 8 .* out ./ (N1 * N2 * N3);
end

function out = DCT1D(x, dim)
    N = size(x, dim);
    sz = ones(1,3);
    sz(dim) = N;
    index = {':',':',':'};
    out = zeros(size(x));
    for n = 0:N-1
       alpha = cos(pi/(2*N)*(2*(0:N-1)+1)*n);
       alpha = reshape(alpha, sz);
       index{dim} = n + 1;
       sum_result = sum(x .* alpha, dim);
       if n == 0
           sum_result = sum_result / sqrt(2);
       end
       out(index{:}) = sum_result;
    end
end
