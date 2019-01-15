import subprocess

# Runs command to start data bus
s = "sudo /sbin/ip link set can0 up type can bitrate 125000"
p = subprocess.Popen(s, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
# Echo eventual errors
res = p.stdout.readlines()
if res:
	print "Error:"
	for line in res:
		print line
# Echo system initiation
else:
	print "System intitiated"
retval = p.wait()