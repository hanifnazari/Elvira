<DOCTYPE html>
<html>
	<head>
		<style>
			body{
				background-color:rgb(75,75,75);
			}
			h1{
				float:center;
				width:80%;
				color:rgba(0,0,0,0.7);
				background-color:rgba(255,255,255,0.9);
				border:4px solid rgba(0,0,0,0.5);
				text-align:center;
				padding:15px;
				margin-left:2%;
				margin-right:2%;
			}
			p{
				font-size:20px;
				padding-left:5px;
				padding-top:1px;
				padding-bottom:1px;
				margin:5px;
			}
			textarea{
				float:right;
				margin-right:10px;
			}
			input.text{
				vertical-align:top;
				float:right;
				margin-right:10px;
			}
			nav{
				float:left;
				width:80%;
				color:rgba(0,0,0,0.7);
				background-color:rgba(255,255,255,0.9);
				border:2px solid rgba(0,0,0,0.5);
				padding:16px;
				margin-left:2%;
				margin-right:2%;
			}
		</style>
	</head>

	<body>

		<h1>Elvira Control Panel</h1>

		<section>
			<nav><form method="post">
				<p>Status :
					<textarea id="status" style="resize:none; overflow:hidden" rows="4" cols="20" readonly><?php
						if(isset($_POST['send']))
						{
							$command = "python /var/www/html/sendMsg.py 0";
							$arg1 = $_POST['input1'];
							$arg2 = $_POST['input2'];
							$arg3 = $_POST['input3'];
							$arg4 = $_POST['input4'];
							if($arg1){
								$command = $command . ' ' . $arg1;
							} else {$command = $command . ' 0';}
							if($arg2){
								$command = $command . ' ' . $arg2;
							} else {$command = $command . ' 0';}
							if($arg3){
								$command = $command . ' ' . $arg3;
							} else {$command = $command . ' 0';}
							if($arg4){
								$command = $command . ' ' . $arg4;
							} else {$command = $command . ' 0';}
							echo shell_exec($command);
						}
						if(isset($_POST['init']))
						{
							echo shell_exec("python /var/www/html/startCan.py");
						}
						?>					
					</textarea></p>
				<p style="margin-top: 45px">Motor 1:
					<input class="text" type="text" name="input1" value="" style="width:30px"/>
					</p>
				<p>Motor 2:  
					<input class="text" type="text" name="input2" value="" style="width:30px"/>
					</p>
				<p>Motor 3:  
					<input class="text" type="text" name="input3" value="" style="width:30px"/>
					</p>
				<p>Motor 4:  
					<input class="text" type="text" name="input4" value="" style="width:30px"/>
					</p>
				<p style="float:left">
					<input type="submit" name="init" value="Init System" style="width:80px; padding:6px"/>
					<input type="submit" name="send" value="Send" style="width:60px; padding:6px; margin-left:10px"/>
					</p>
			</form></nav>
		</section>
	</body>
</html>