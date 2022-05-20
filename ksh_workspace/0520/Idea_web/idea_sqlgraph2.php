<?php
$conn2 = mysqli_connect("localhost","root","kcci");

mysqli_set_charset($conn2,"utf8");
mysqli_select_db($conn2,"test");



$result2 = mysqli_query($conn2,"select ID,date,time from Idea");

$data2 = array(array('test','예약 횟수'));

if($result2){
	while($row=mysqli_fetch_array($result2)){
		array_push($data2,array($row[1]."\n".$row[2],intval($row[0]))); // data라는 변수에한 줄씩 값을 집어넣음. $row['DATE'] 처럼 이름을 가져와도 되고$row[1] 처럼 요소로 가져와도 된다.
	}											// .은 문자열을 이어붙이는 용도
}

$options2 = array(
	'title' => '전체 이용 횟수',
	'width' => 1000,'height' => 500
);
?>


<script src="//www.google.com/jsapi"></script>
<script>
    let data = <?= json_encode($data2) ?>;
    let options = <?= json_encode($options2) ?>;
    google.load('visualization', '1.0', {'packages':['corechart']});
    google.setOnLoadCallback(function() {
        let chart = new google.visualization.ColumnChart(document.querySelector('#chart_div'));
        chart.draw(google.visualization.arrayToDataTable(data), options);
    });
</script>
<div id="chart_div"></div>

