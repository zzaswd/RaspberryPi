<!DOCTYPE  html>
<html>
<head>
    <meta charset = "UTF-8">
    <meta http-equiv = "refresh" content = "30">
    <style type = "text/css">
   		.spec{
        	text-align:center;
       	}
        .con{
        	text-align:left;
        }
    </style>
</head>

<body>
	<h1 align = "center"> 회원 정보</h1>
    <div class = "spec">
    	# <b> 회원가입 유저 정보</b><br>
        # 가입 순서 아이디 비밀번호 날짜와 시간 <br>
    </div>

	<table border = '1' style = "width = 30%" align = "center">
	<tr align = "center">
		<th>NUM</th>
		<th>ID</th>
		<th>PASS</th>
		<th>DATE</th>
		<th>TIME</th>
	</tr>
	<?php
		$conn = mysqli_connect("localhost","root","kcci");
		mysqli_select_db($conn,"raspi_project");
		$result = mysqli_query($conn, "select * from User_data");

		while($row = mysqli_fetch_array($result)){
			echo "<tr align = center>";
			echo '<td>'.$row['NUM'].'</td>';
			echo '<td>'.$row['ID'].'</td>';
			echo '<td>'.$row['PASS'].'</td>';
			echo '<td>'.$row['DATE'].'</td>';
			echo '<td>'.$row['TIME'].'</td>';
			echo "</tr>";
			mysqli_close($conn);
		}

	?>
	</table>

  	<form method= "get" action="raspi_table.php">
		<input type = "submit" value ="ON" name ="ON">
		<input type = "submit" value ="OFF" name ="OFF"><br><br><br>
   	</form>
	
	
	<?php
	if(isset($_GET['ON'])){
		$fp = fopen("/var/www/html/cam_status.txt",'w');
		fwrite($fp,"ON");
		fclose($fp);
		echo "CAM_ON";
	}
	else if(isset($_GET['OFF'])){
		$fp = fopen("/var/www/html/cam_status.txt",'w');
		fwrite($fp,"OFF");
		fclose($fp);
		echo "CAM_OFF";
	}
	?>


  	<form method= "get" action="raspi_table.php">
		<input type = "submit" value ="LEFT" name ="LEFT">
		<input type = "submit" value ="MIDDLE" name ="MIDDLE">
		<input type = "submit" value ="RIGHT" name ="RIGHT"><br><br><br>
   	</form>
	<?php
	if(isset($_GET['LEFT'])){
		$fp = fopen("/var/www/html/cam_control.txt",'w');
		fwrite($fp,"LEFT");
		fclose($fp);
		echo "LEFT";
	}
	else if(isset($_GET['MIDDLE'])){
		$fp = fopen("/var/www/html/cam_control.txt",'w');
		fwrite($fp,"MIDDLE");
		fclose($fp);
		echo "MIDDLE";
	}
	else if(isset($_GET['RIGHT'])){
		$fp = fopen("/var/www/html/cam_control.txt",'w');
		fwrite($fp,"RIGHT");
		fclose($fp);
		echo "RIGHT";
	}

	?>



</body>

</html>
