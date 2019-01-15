import subprocess
import sys

# ========================================#
# Loops through all args		          #
# Sets msg id (not using extended can id) #
# Sets msg type	(first byte)		      #
# 00 -> send angle			              #
# 	Angle between 1-255 (second byte)     #
# 01 -> request angle			          #
#	No arg				                  #
# 03 -> change max angle	              #
# 	Angle between 1-255 (second byte)     #
# 03 -> change min angle	              #
# 	Angle between 1-255 (second byte)     #
# Sets msg				                  #
# Sends msg				                  #
#					                      #
# Arguments:				              #
# msgType argId1 argId2 argId3 ...	      #
#					                      #
# Message form:				              #
# func	  dev  id #msg(max 8 bytes)	      #
# cansend can0 000#0000000000000000	      #
# ========================================#

# Checks if message type is valid
if sys.argv[1].isdigit() and int(sys.argv[1]) >= 0 and int(sys.argv[1]) <= 99 and not sys.argv[1].isspace():
	# Format message typ
	type = str(sys.argv[1])
	if len(type) == 1:
		type = "0"+type
	# Loops all messages
	for i in range(2,len(sys.argv)):
		# Checks if message is valid
		if sys.argv[i].isdigit() and int(sys.argv[i]) > 0 and int(sys.argv[i]) <= 255 and not sys.argv[i].isspace():
			# Format id 
			id = str(i-1)
			while len(id) < 3:
				id = "0"+id
			# Format message
			msg = str(hex(int(sys.argv[i]))[2:])
			if len(msg) == 1:
				msg = "0"+msg
			# Format command argument
			s = 'cansend can0 '+id+'#'+type+msg
			# Run command argument
			p = subprocess.Popen(s, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
			# Echo evetual errors
			res = p.stdout.readlines()
			if res:
				print "Error:"
				for line in res:
					print line
			# Echo send confirmation
			else:
				print("sent: " + sys.argv[i] + " to id " + id)
			retval = p.wait()
		# Echo argument out of bounds
		if int(sys.argv[i]) < 0 or int(sys.argv[i]) > 255:
			print("Error: out of bounds")
		# Echo argument invlid
		else
			print("Error: argument invalid")
# Echo invalid message type
else 
	print("Error: invalid message type")