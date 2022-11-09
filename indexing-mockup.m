## the situation
BM = [5, 4, 1, 1]; BQ = [1, 5, 20, 20];
AM = [4, 5, 1, 1]; AQ = [1, 4, 20, 20];

## model parameters
Gm = 5; Gn = 4;                      # global invoke range
Hm = 2; Hn = 2;                      # group size (these don't matter)

## work item indices
g = 0:(Gm * Gn - 1);                # linear integer id
G = [mod(g, Gm); floor(g / Gm)];    # 2d id    .. both from oneapi

#{
global linear id: buffer<T,2> (3,2)
        0   1
        2   3
        4   5
id[0] buffer<T,2> (3,2)
        0   0
        1   1
        2   2
id[1] buffer<T,2> (3,2)
        0   1
        0   1
        0   1
#}

## A([i,j]) = B([j,i])
## T(g) = [i,j]
## A(T(g)) = B([0 1; 1 0] * T(g))
## a(A_*T(g)) = b(B_ * [0 1; 1 0] * T(g))
## A_*T(g) = B_ * [0 1; 1 0] * T(g)
## F(g):G=>Bz F(g) = B_ * [0 1; 1 0] * T(g)
## U:G=>Bz   (implicitly)
## pick T(g) so that F(g)=U(g)
