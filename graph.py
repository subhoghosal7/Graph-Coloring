


import sys

with open(sys.argv[1], "r") as f:
	for t in f:
		if(t.startswith('p')):
			n,e = [ int(i) for i in t.rstrip('\n').split(' ') if(i.isdigit())]
			print(n, ' ', e)
		elif(t.startswith('e')):
			x,y = [ int(i) for i in t.rstrip('\n').split(' ') if(i.isdigit())]
			print(x,' ',y)












