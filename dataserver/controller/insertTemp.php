<?php

/*
 * class InsertTemperature
 */ 
 
include './model/Temperature.php';

$class_temp = new InsertTemperature($_GET["temperatura"], $_GET["id_node"]);


class InsertTemperature{	
	
	function InsertTemperature($temperature, $id_node){
		$this->setTemperature($temperature, $id_node);
		
	}

	function setTemperature($temperature, $id_node){
		
		$temp = new Temperature();
		
		$temp->insertTemperature($temperature, $id_node);

		$temp->closeConection();
	
	}	
}

?>