<html>

<body>

<form method="post">
	<p>LED Status</p>
	<textarea name="text" style="resize:none"><?php
		if(isset($_POST['on']))
		{
			echo shell_exec("python /var/www/LED/ledON.py");
		}
		if(isset($_POST['off']))
		{
			echo shell_exec("python /var/www/LED/ledOFF.py");
		}
		if(isset($_POST['switch']))
		{
			echo shell_exec("python /var/www/LED/ledSWITCH.py");
		}
		?>
	</textarea>

	<p>LED On/Off</p>
	<input type="submit" name="on" value="ON"/>
	<input type="submit" name="off" value="OFF"/>

	<p>LED Switch</p>
	<input type="submit" name="switch" value="SWITCH"/>
</form>
</body>
</html>