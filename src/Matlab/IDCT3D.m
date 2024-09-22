function out = IDCT3D(X)
    out = IDCT1D(X, 1);
    out = IDCT1D(out, 2);
    out = IDCT1D(out, 3);
end

function out = IDCT1D(X, dim)
    N = size(X, dim);
    sz = ones(1,3);
    sz(dim) = N;
    index = {':',':',':'};
    out = zeros(size(X));
    for n = 0:N-1
       alpha = cos(pi/(2*N)*(2*n+1)*(0:N-1));
       alpha(1) = alpha(1) / sqrt(2);
       alpha = reshape(alpha, sz);
       index{dim} = n + 1;
       out(index{:}) = sum(X .* alpha, dim);
    end
end
