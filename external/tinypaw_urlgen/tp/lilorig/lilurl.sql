-- MySQL dump 8.22
--
-- Host: localhost    Database: lilurl
---------------------------------------------------------
-- Server version	3.23.57

--
-- Table structure for table 'lil_urls'
--

CREATE TABLE lil_urls (
  id varchar(255) NOT NULL default '',
  url text,
  date timestamp(14) NOT NULL,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table 'lil_urls'
--



