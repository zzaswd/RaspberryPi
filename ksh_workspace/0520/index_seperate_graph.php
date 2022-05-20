<?php
$conn = mysqli_connect("localhost","root","kcci");
mysqli_set_charset($conn,"utf8");
mysqli_select_db($conn,"test");
$result = mysqli_query($conn,"select DATE,TIME,TEMPERATURE from sensing");

$data = array(array('test','온도'));
if($result){
	while($row=mysqli_fetch_array($result)){
		array_push($data,array($row['DATE']."\n".$row[1],intval($row[2]))); // data라는 변수에한 줄씩 값을 집어넣음. $row['DATE'] 처럼 이름을 가져와도 되고$row[1] 처럼 요소로 가져와도 된다.
	}											// .은 문자열을 이어붙이는 용도
}

$options = array(
	'title' => '온도 (단위 : 섭씨)',
	'width' => 1000,'height' => 500
);
?>




<script src="//www.google.com/jsapi"></script>
<script>
    let data = <?= json_encode($data) ?>;
    let options = <?= json_encode($options) ?>;
    google.load('visualization', '1.0', {'packages':['corechart']});
    google.setOnLoadCallback(function() {
        let chart = new google.visualization.ColumnChart(document.querySelector('#chart_div'));
        chart.draw(google.visualization.arrayToDataTable(data), options);
    });
</script>
<div id="chart_div"></div>

