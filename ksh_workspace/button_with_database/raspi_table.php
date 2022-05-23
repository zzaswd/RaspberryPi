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
  	<form action="/var/www/html/raspi_on.php" method="get">
    	<button type="submit" formaction="http://192.168.0.6/raspi_on.php">on</button>
   	</form>


  	<form action="/var/www/html/raspi_off.php" method="get">
    	<button type="submit" formaction="http://192.168.0.6/raspi_off.php">off</button>
   	</form>



</table>
<body>

</html>
