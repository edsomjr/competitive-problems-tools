import sys

lines = sys.stdin.readlines()

N, M = lines[0].split();

N = int(N)
M = float(M)
xs = [float(x) for x in lines[1].split()]
ys = sorted(zip(xs, range(1, N + 1)))

ans = []

for y, k in ys:
    if y <= M:
        ans.append(str(k))
        M -= y

print len(ans)

if len(ans) > 0:
    print ' '.join(ans)
