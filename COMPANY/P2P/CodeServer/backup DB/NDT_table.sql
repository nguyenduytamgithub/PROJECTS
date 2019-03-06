-- phpMyAdmin SQL Dump
-- version 4.2.12deb2+deb8u5
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Mar 06, 2019 at 09:38 AM
-- Server version: 5.5.62-0+deb8u1
-- PHP Version: 5.6.40-0+deb8u1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `NDT_table`
--

-- --------------------------------------------------------

--
-- Table structure for table `mqtt_in`
--

CREATE TABLE IF NOT EXISTS `mqtt_in` (
`unit` int(4) NOT NULL,
  `id_mqtt_in` varchar(15) NOT NULL,
  `x` int(5) NOT NULL,
  `y` int(5) NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf32;

--
-- Dumping data for table `mqtt_in`
--

INSERT INTO `mqtt_in` (`unit`, `id_mqtt_in`, `x`, `y`) VALUES
(1, 'id_mqtt_in1', 120, 420);

-- --------------------------------------------------------

--
-- Table structure for table `server_connected`
--

CREATE TABLE IF NOT EXISTS `server_connected` (
`unit` int(4) NOT NULL,
  `id_server` varchar(15) NOT NULL,
  `status` int(1) NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf32;

-- --------------------------------------------------------

--
-- Table structure for table `server_dht`
--

CREATE TABLE IF NOT EXISTS `server_dht` (
`unit` int(4) NOT NULL,
  `id_server` varchar(15) NOT NULL,
  `ip_server` varchar(15) DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf32;

--
-- Dumping data for table `server_dht`
--

INSERT INTO `server_dht` (`unit`, `id_server`, `ip_server`) VALUES
(1, 'p2p_elefos001', '115.79.143.47'),
(3, 'p2p_elefos002', '68.183.234.96'),
(2, 'p2p_elefos003', '157.230.34.0');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `mqtt_in`
--
ALTER TABLE `mqtt_in`
 ADD PRIMARY KEY (`id_mqtt_in`), ADD UNIQUE KEY `unit` (`unit`);

--
-- Indexes for table `server_connected`
--
ALTER TABLE `server_connected`
 ADD PRIMARY KEY (`id_server`), ADD UNIQUE KEY `unit` (`unit`);

--
-- Indexes for table `server_dht`
--
ALTER TABLE `server_dht`
 ADD PRIMARY KEY (`id_server`), ADD UNIQUE KEY `unit` (`unit`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `mqtt_in`
--
ALTER TABLE `mqtt_in`
MODIFY `unit` int(4) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=7;
--
-- AUTO_INCREMENT for table `server_connected`
--
ALTER TABLE `server_connected`
MODIFY `unit` int(4) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=7;
--
-- AUTO_INCREMENT for table `server_dht`
--
ALTER TABLE `server_dht`
MODIFY `unit` int(4) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=4;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
