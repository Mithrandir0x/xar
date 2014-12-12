<?php

/*
 * class Temperature
 */ 

include './config/BBDDConector.php';

class Temperature{
	
	private $bd;
	
	function Temperature(){		
		$this->bd = new BBDDConector();		
	}
	
	function getAllTemperatures(){
		$temps = array();				
		$query = 'SELECT * FROM bd_temp';
	
		$result = $this->bd->bbdd_link->query($query) or die('<br> -- Consulta fallida: ' . mysql_error() . '<br>');
		
		while($row = $result->fetch_array()) {			
			array_push($temps, $row);
		}
		return $temps;		
		
	}	
	
	function insertTemperature($temperature, $id_node){
		$query = 'INSERT INTO bd_temp(temperature, insert_date, id_node) VALUES (' . $temperature . ', NOW() + INTERVAL 6 HOUR, '. $id_node . ' )';

		$result = $this->bd->bbdd_link->query($query) or die('Consulta fallida: ' . mysql_error());
		
	}
		
	function closeConection(){
		$this->bd->bbdd_link->close();
	}
}

?>