-- phpMyAdmin SQL Dump
-- version 5.0.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: May 15, 2020 at 08:01 PM
-- Server version: 10.4.11-MariaDB
-- PHP Version: 7.4.1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `tele_db`
--

-- --------------------------------------------------------

--
-- Table structure for table `admin`
--

CREATE TABLE `admin` (
  `id` int(11) NOT NULL,
  `email` varchar(150) DEFAULT NULL,
  `password` varchar(200) DEFAULT NULL,
  `hash` varchar(200) DEFAULT NULL,
  `lastSeen` varchar(50) DEFAULT NULL,
  `loginTime` varchar(50) DEFAULT NULL,
  `created_at` varchar(50) DEFAULT NULL,
  `updated_at` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `admin`
--

INSERT INTO `admin` (`id`, `email`, `password`, `hash`, `lastSeen`, `loginTime`, `created_at`, `updated_at`) VALUES
(1, 'admin@gmail.com', '$2y$13$hIuBPigx9R6hGyUbVQcgUe3f6YUP./GJh.VEQSgTlcP2PX8k9g23O', 'admin_1589458634', '1589458635', '1589458635', NULL, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `brands`
--

CREATE TABLE `brands` (
  `id` int(11) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `image` varchar(200) DEFAULT NULL,
  `created` varchar(50) DEFAULT NULL,
  `updated` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `brands`
--

INSERT INTO `brands` (`id`, `name`, `image`, `created`, `updated`) VALUES
(1, 'Tecno', '2020/May/14/brand_1589460462.jpg', '1589279847', '1589460467'),
(2, 'Xamoi', '2020/May/14/brand_1589411519.jpg', '1589411527', NULL),
(3, 'Samsung', '2020/May/14/brand_1589411541.jpg', '1589411545', NULL);

-- --------------------------------------------------------

--
-- Table structure for table `customers`
--

CREATE TABLE `customers` (
  `id` int(11) NOT NULL,
  `firstname` varchar(50) DEFAULT NULL,
  `lastname` varchar(50) DEFAULT NULL,
  `email` varchar(50) NOT NULL,
  `password` varchar(50) NOT NULL,
  `hash` varchar(50) DEFAULT NULL,
  `lastSeen` varchar(50) DEFAULT NULL,
  `loginTime` varchar(50) DEFAULT NULL,
  `created` varchar(50) DEFAULT NULL,
  `updated` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `customer_logger`
--

CREATE TABLE `customer_logger` (
  `id` int(11) NOT NULL,
  `hash` varchar(50) NOT NULL,
  `timemodifier` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `models`
--

CREATE TABLE `models` (
  `id` int(11) NOT NULL,
  `name` varchar(100) NOT NULL,
  `brand` int(11) NOT NULL,
  `price` varchar(50) NOT NULL,
  `description` varchar(300) NOT NULL,
  `banner` varchar(50) NOT NULL,
  `sideTop` varchar(50) NOT NULL,
  `sideDown` varchar(50) NOT NULL,
  `sideFront` varchar(50) NOT NULL,
  `sideBack` varchar(50) NOT NULL,
  `sideLeft` varchar(50) NOT NULL,
  `sideRight` varchar(50) NOT NULL,
  `features` text NOT NULL,
  `created` varchar(50) NOT NULL,
  `updated` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `models`
--

INSERT INTO `models` (`id`, `name`, `brand`, `price`, `description`, `banner`, `sideTop`, `sideDown`, `sideFront`, `sideBack`, `sideLeft`, `sideRight`, `features`, `created`, `updated`) VALUES
(2, 'Hot 6', 3, '', '', '', '', '', '', '', '', '', '', '1589411562', '1589463470'),
(3, 'WX3', 2, '', '', '', '', '', '', '', '', '', '', '1589411580', '1589466115'),
(4, 'Pro Book', 1, '', '', '', '', '', '', '', '', '', '', '1589466136', '1589466824'),
(5, 'Hot 5', 3, '3000', 'jjgkjguguiguigi', '2020/May/15/brand_1589564183.jpg', '2020/May/15/brand_1589564194.jpg', '2020/May/15/brand_1589564197.jpg', '2020/May/15/brand_1589564188.jpg', '2020/May/15/brand_1589564191.jpg', '2020/May/15/brand_1589564200.jpg', '2020/May/15/brand_1589564203.jpg', '', '1589564886', NULL),
(6, 'Hot 5', 1, '3000', 'mjbguigui', '2020/May/15/brand_1589565338.jpg', '2020/May/15/brand_1589565347.jpg', '2020/May/15/brand_1589565352.jpg', '2020/May/15/brand_1589565342.jpg', '2020/May/15/brand_1589565345.jpg', '2020/May/15/brand_1589565359.jpg', '2020/May/15/brand_1589565363.jpg', '{\"dffs\":\"jugig\"}', '1589565393', NULL),
(7, 'Pro', 2, '3000', 'iffofuguyg', '2020/May/15/brand_1589565479.jpg', '2020/May/15/brand_1589565496.jpg', '2020/May/15/brand_1589565482.jpg', '2020/May/15/brand_1589565484.jpg', '2020/May/15/brand_1589565489.jpg', '2020/May/15/brand_1589565492.jpg', '2020/May/15/brand_1589565499.jpg', '{\"features\":{\"dffs\":\"afssafa\",\"jjvjhvkj\":\"jbkjbb\",\"guguj\":\"jjbbjk\"}}', '1589565571', NULL);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `admin`
--
ALTER TABLE `admin`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `brands`
--
ALTER TABLE `brands`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`name`);

--
-- Indexes for table `customers`
--
ALTER TABLE `customers`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `customer_logger`
--
ALTER TABLE `customer_logger`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `models`
--
ALTER TABLE `models`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `admin`
--
ALTER TABLE `admin`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `brands`
--
ALTER TABLE `brands`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- AUTO_INCREMENT for table `customers`
--
ALTER TABLE `customers`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `models`
--
ALTER TABLE `models`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
