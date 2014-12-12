<?php

/*
 * class Temperature 
 * controller
 */ 

include './model/Temperature.php';

$class_temp = new showTemperature();


class showTemperature{

	var $temperatures;

	function showTemperature(){

		//select
		$this->getTemperatures();

		$this->DisplayTemplate();		
		
	}	

	function getTemperatures(){
		
		$temp = new Temperature();
		$this->temperatures = $temp->getAllTemperatures();

		$temp->closeConection();
	
	}
	
	function DisplayTemplate(){
		include("./views/temp_table.php");
		
	}
	
}

?>