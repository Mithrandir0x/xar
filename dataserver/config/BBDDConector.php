<?php

class BBDDConector{
		
		public $bbdd_link;
		private $servername = "mysql1.000webhost.com";
		private $username = "a8795730_user";
		private $password = "Pimpampum1";
		private $dbname = "a8795730_bbdd";
		
		function BBDDConector(){
							
			$this->bbdd_link = new mysqli($this->servername, $this->username, $this->password, $this->dbname) or die('No pudo conectarse: ' . mysql_error());
			
		}
	
}


?>