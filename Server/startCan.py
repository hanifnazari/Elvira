import subprocess

s = "sudo /sbin/ip link set can0 up type can bitrate 125000"
p = subprocess.Popen(s, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

res = p.stdout.readlines()
if res:
	print "Error:"
	print res
	for line in p.stdout.readlines():
		print line
else:
	print "System intitiated"
retval = p.wait()