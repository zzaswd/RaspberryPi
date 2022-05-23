<!DOCTYPE  html>
<html>
<head>
    <meta charset = "UTF-8">
    <meta http-equiv = "refresh" content = "0; url = /raspi_table.php">
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
	<h1 align = "center"> 카메라 On Off</h1>
    <div class = "spec">
    	# <b> 웹 카메라 온</b><br>
        # On 		OFF <br>
    </div>

	<table border = '1' style = "width = 30%" align = "center">
	<tr align = "center">
		<th>STATUS</th>
	</tr>
	
	<?php
		$conn = mysqli_connect("localhost","root","kcci");
		mysqli_select_db($conn,"raspi_project");
		$result = mysqli_query($conn, "update ON_OFF set status = 'off' where status = 'on'");
		mysqli_close($conn);
	?>




</table>
<body>

</html>
