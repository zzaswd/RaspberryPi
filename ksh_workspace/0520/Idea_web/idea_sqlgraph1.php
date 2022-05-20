<?php
$conn1 = mysqli_connect("localhost","root","kcci");
mysqli_set_charset($conn1,"utf8");
mysqli_select_db($conn1,"test");


$result1 = mysqli_query($conn1,"select NOW_BUS_STOP,TO_BUS_STOP, BUS_NUMBER,date,time from Idea");

$data1 = array(array('test','버스번호'));
if($result1){
	while($row=mysqli_fetch_array($result1)){
		if(strcmp($row[0],"Uzangsan") == 0 && strcmp($row[1],"hwagok") == 0)
		array_push($data1,array($row[3]."\n".$row[4],intval($row[2]))); // data라는 변수에한 줄씩 값을 집어넣음. $row['DATE'] 처럼 이름을 가져와도 되고$row[1] 처럼 요소로 가져와도 된다.
	}											// .은 문자열을 이어붙이는 용도
}

$options1 = array(
	'title' => '정류장 : 우장산 to 화곡',
	'width' => 1000,'height' => 500
);
?>


<script src="//www.google.com/jsapi"></script>
<script>
    let data = <?= json_encode($data1) ?>;
    let options = <?= json_encode($options1) ?>;
    google.load('visualization', '1.0', {'packages':['corechart']});
    google.setOnLoadCallback(function() {
        let chart = new google.visualization.ColumnChart(document.querySelector('#chart_div'));
        chart.draw(google.visualization.arrayToDataTable(data), options);
    });
</script>
<div id="chart_div"></div>

