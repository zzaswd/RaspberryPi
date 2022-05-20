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
	<h1 align = "center"> My Database</h1>
	<div class = "spec">
		# <b> 현재 대중교통 예약 현황</b><br>
		#이용 회수   현재 정류장    다음 정류장    버스번호 <br>
	</div>

	<table border = '1' style = "width = 30%" align = "center">
	<tr align = "center">
		<th>ID</th>
		<th>NOW_BUS_STOP</th>
		<th>TO_BUS_STOP</th>
		<th>BUS_NUMBER</th>
		<th>DATE</th>
		<th>TIME</th>
	</tr>
	<?php
		$conn = mysqli_connect("localhost","root","kcci");
		mysqli_select_db($conn,"test");
		$result = mysqli_query($conn, "select * from Idea");

		while($row = mysqli_fetch_array($result)){

			echo "<tr align = center>";
			echo '<td>'.$row['ID'].'</td>';
			echo '<td>'.$row['NOW_BUS_STOP'].'</td>';
			echo '<td>'.$row['TO_BUS_STOP'].'</td>';
			echo '<td>'.$row['BUS_NUMBER'].'</td>';
			echo '<td>'.$row['date'].'</td>';
			echo '<td>'.$row['time'].'</td>';
			echo "</tr>";
			mysqli_close($conn);
		}

	?>
	</table><body></html>
		



		


