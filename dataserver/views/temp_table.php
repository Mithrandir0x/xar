<!-- table with all temperatures -->		
<!DOCTYPE html>
<head>
	<title>Temperatures table</title>
	<link rel="stylesheet" href="./css/styles.css" type="text/css">
</head>
<body>
    <table id="temperatures_table">
    <tr><td><?php echo "<br>Total Rows: " . count($this->temperatures) . "<br>"; ?></td></tr>
    <tr class="table_headers"><td>ID Node</td><td>Temperature</td><td>Insert Date</td></tr>
    
    <?php
            foreach ($this->temperatures as &$value) {
                echo "<tr class='table_row'><td>" . $value["id_node"] . "</td><td>" . $value["temperature"] . "</td><td>" . $value["insert_date"] . "</td></tr>";
            }
    ?>
    </table>
</body>
<!-- end table with all temperatures -->