#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#define max(m, n) ((m) > (n) ? (m) : (n))
 int s , n , k , p;
 
typedef struct pair
{
    int x;
    long y;
} pair;
 
long ipow(int x, int n)
{
    if(n == 0) return 1;
    if(n % 2 == 0) return ipow(x * x, n / 2);
    return x * ipow(x, n - 1);
}
 
void euclid(int a, int b, int *d, int *x, int *y)
{
    if (b == 0)
    {
        *d = a;
        *x = 1;
        *y = 0;
    }
    else {
        int x0, y0;
        euclid(b, a % b, d, &x0, &y0);
        *x = y0;
        *y = x0 - (a / b) * y0;
    }
}
 
 
long eval(int* P, int x, int n)
{
    assert(P != NULL);
    assert(n >= 0);
    int res = 0;
    int i;
    for(i=0; i<=n; i++)
        res += P[i] * ipow(x, i);
    return res;
}
 
pair* encrypt(int s, int n, int k, int p)
{
    assert(k <= n);
    assert(p > max(s, n));
    int* P = malloc(k * sizeof(int));
    if(P == NULL)
        exit(1);
    int i;
    P[0] = s;
    for(i=1; i<=k-1; i++)
        P[i] = rand() % p;
    pair* out = malloc(n * sizeof(pair));
    if(out == NULL)
        exit(1);
    for(i=0; i<n; i++)
    {
        out[i].x = rand() % p;
        int unique = 0;
        while(!unique)
        {
            unique = 1;
            int j;
            for(j=0; j<i; j++)
                if(out[j].x == out[i].x)
                {
                    unique = 0;
                    out[i].x = rand() % p;
                    break;
                }
        }
        out[i].y = eval(P, out[i].x, k-1) % p;
    }
    return out;
}
 
int decrypt(pair* pairs, int k, int p)
{
    assert(p > k);
    assert(pairs != NULL);
    int s= 0;
    int i, j;
    for(i=0; i<k; i++)
    {
        int prod = pairs[i].y;
        for(j=0; j<k; j++)
        {
            if(j == i) continue;
            prod*= pairs[j].x;
            int d, x, y;
            euclid(p, (pairs[j].x - pairs[i].x + p) % p, &d, &x, &y);
            prod *= (y + p) % p;
            prod %= p;
        }
        s += prod;
    }
    s %= p;
 return s;

}
 
int main(int argc, char **argv)
{
    srand(time(NULL));
    printf("Enter the value of s");
     scanf("%d",&s);
    printf("Enter the value of n");  
     scanf("%d",&n);
    printf("Enter the value of k");
     scanf("%d",&k);
    printf("Enter the value of p");
     scanf("%d",&p);
    pair* pairs = encrypt(s, n, k, p);
    int i;
    for(i=0; i<n; i++)
        printf("(%d, %ld)\n", pairs[i].x, pairs[i].y);
    printf("Reconstructed secret key is:");
    printf("\nS = %d", decrypt(pairs, n, p));
    printf("\n");
    free(pairs);
    return 0;
  
}
