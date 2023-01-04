-- MySQL dump 10.13  Distrib 8.0.19, for Win64 (x86_64)
--
-- Host: 127.0.0.1    Database: x_wallet
-- ------------------------------------------------------
-- Server version	8.0.26

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `address`
--

DROP TABLE IF EXISTS `address`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `address` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `wallet_id` int NOT NULL,
  `currency` varchar(200) NOT NULL,
  `label` varchar(250) DEFAULT NULL,
  `coinSpecific` varchar(255) DEFAULT NULL,
  `addressType` varchar(255) DEFAULT NULL,
  `address` varchar(150) NOT NULL,
  `is_external` tinyint(1) NOT NULL DEFAULT '0',
  `is_hd` tinyint(1) NOT NULL DEFAULT '0',
  `hd_path` varchar(100) DEFAULT NULL,
  `secret` text NOT NULL,
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  `user_id` int NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `IDX_843cd1381bf922ba08da8160fa` (`addressType`),
  KEY `wallet_id` (`wallet_id`)
) ENGINE=InnoDB AUTO_INCREMENT=110 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `address`
--

LOCK TABLES `address` WRITE;
/*!40000 ALTER TABLE `address` DISABLE KEYS */;
INSERT INTO `address` VALUES (1,1,'eth','stridsdsdsdng',NULL,NULL,'0x2a7Ee999bda4cF1A13B2f7a4c71Ad055d7F72784',0,0,NULL,'{\"private_key\":\"0x96a9a5011a722e4229d7a939085b28cb2a5e8518517ffb4b3c3612e13b79211e\",\"kms_data_key_id\":0}',1634284685468,1634670218482,1),(2,1,'eth','Test address eth',NULL,NULL,'0xDc4CD486bfF6EB5356f3D12ac274cAD46c4ce531',0,0,NULL,'{\"private_key\":\"0x509c2b55686b5193abd3dfe8757c47421a287d20dfb035ba88e184c1f8b93a50\",\"kms_data_key_id\":0}',1634284701555,1634284701555,1),(3,2,'btc','Test address eth',NULL,NULL,'mnNC3VAeorZsWe8x6oNmc35Mgv8JLgQuze',0,0,NULL,'{\"private_key\":\"cRSs5nZ63KwytQj15RbCZ7ytUQnmx8rpcmtK1o2MYBpc9WXBeL1E\",\"kms_data_key_id\":0}',1634284711923,1634284711923,1),(4,1,'eth','123',NULL,NULL,'0xee479478Ad0EdF2E42D86586386bcddf58df2ef6',0,0,NULL,'{\"private_key\":\"0xcc9b149e37fc0c11a1d844639af5caab59880339804f33a6fa86c85aebd1ab0b\",\"kms_data_key_id\":0}',1634284748428,1634284748428,1),(5,1,'eth','string',NULL,NULL,'0xf693539251e5fA96459E4241BBA06533d0011AaF',0,0,NULL,'{\"private_key\":\"0x16ddfe86035fb24304bee3f8d4084ad176fd23556477d119114662fda1590f37\",\"kms_data_key_id\":0}',1634284839719,1634284839719,1),(6,2,'btc','string',NULL,NULL,'mgJLA94LWCQdBXjRYSCyvBTVr4thYs2b4b',0,0,NULL,'{\"private_key\":\"cQJN62keXUTW9fidyS8hqEDTPRvUxooNaPoPCgzE8BCVEUwst5ay\",\"kms_data_key_id\":0}',1634284850126,1634284850126,1),(7,2,'btc','',NULL,NULL,'n3TWUvJ2btaFzNVDrp7hcweeV7mupeLWvU',0,0,NULL,'{\"private_key\":\"cTWsiwoC4XX5TRk7LskivDzUr9WahwnQVDLVDkxNzLRxCWp2cD2Q\",\"kms_data_key_id\":0}',1634287236068,1634287236068,1),(8,2,'btc','</br>!@#$%^&*()_+_',NULL,NULL,'mrwcMeWvgj9Jfdb4MsxXGbE9d2BHEvPrCS',0,0,NULL,'{\"private_key\":\"cUsaFZb8ykzrp7iWstXPbbvfLkXPvk7hQ8jumwysxcQrgZahQR5D\",\"kms_data_key_id\":0}',1634287308482,1634287308482,1),(9,2,'btc',NULL,NULL,NULL,'n3sUNCGzeMnBaBfN6zWy9EhmhnKCzHq39U',0,0,NULL,'{\"private_key\":\"cRx2Krn5nTaEtsTctjYpDcFez6BkjoWzjWy1xcW2Vvj63nXQfdsd\",\"kms_data_key_id\":0}',1634287675498,1634287675498,1),(10,2,'btc',NULL,NULL,NULL,'mh9S8MkRFc7WqmXumc3mhdQSoo5m6Fsu93',0,0,NULL,'{\"private_key\":\"cRUWDetPaniXsPcTk1rPL9Pc8ZsCU6Q7PRXpuZNuWw73uCefVKts\",\"kms_data_key_id\":0}',1634287691977,1634287691977,1),(11,2,'btc',NULL,NULL,NULL,'mqd1XgtyjMHwUvEd7JLcjwBDkvasCwZciU',0,0,NULL,'{\"private_key\":\"cTLRL935SzUEhFTXNidthuETrGtc2GXCM8p8fMzdqouSzKyU1a4H\",\"kms_data_key_id\":0}',1634288686109,1634288686109,1),(12,2,'btc','Wallet1',NULL,NULL,'mfuQgJr3173K7vpdb1SWCSTJhkfaMqMzGL',0,0,NULL,'{\"private_key\":\"cUzN5LfBmBsUxas3mXRnEZgaAughEuttAJSw38cucWZsG55pZtk9\",\"kms_data_key_id\":0}',1634289087362,1634289087362,1),(13,1,'eth','Matic@63434%^cdgvđbvHCBDHBVEvfvv6775chbhdveeyerbergvhbdfvdfvbdhbvdffvdbdbvbhd777r4r4cdbCHSCVDSVDJSVDSV64833ry4JCJDBChchdsnjsnvjsdvdvd09@!#$%^&**ncdshcbvhjbvbvdhvdhdhbvbvbdbvdgvsfjskcnscsdhcbsd36473243285U98Uhhsbcdsbcjsgfscsfjksvnhjsdjfeegut4584y5t7hf',NULL,NULL,'0xdA6095f057351d4C03aF4c4f274d55EbBB17F691',0,0,NULL,'{\"private_key\":\"0xf1c8d3ebfa5f3ff212af645e959679870e70b5b9bc5d041ea46ca4e7f6298674\",\"kms_data_key_id\":0}',1634291913925,1634291913925,1),(14,2,'btc','Wallet BTC',NULL,NULL,'n4WQtCu3B4d5aVcv3umR8tyeTP8SN4o9qB',0,0,NULL,'{\"private_key\":\"cRkihadfLYvqabpeR2XZ3xFkqkehwRC142ymNdfdxUd6tNmyr4du\",\"kms_data_key_id\":0}',1634293050547,1634293050547,1),(15,1,'eth','ABC',NULL,NULL,'0x2782EeD3F2c1405bbe1D7eA86fa7CC938Bc67955',0,0,NULL,'{\"private_key\":\"0xf8493a7066e4e4c4c7fe2dfb024ce73c29146154261159f453543de5c86e88da\",\"kms_data_key_id\":0}',1634523804365,1634523804365,1),(16,1,'eth','ABC',NULL,NULL,'0x88b520B6e4205C7eB3997686681DFE2880e4Ef07',0,0,NULL,'{\"private_key\":\"0xcbe7fdcdb2602d00a328b745c6d6ccf33feb80973b2a3f04299e1bbe3393462b\",\"kms_data_key_id\":0}',1634525428367,1634525428367,1),(17,1,'eth','wallet 1',NULL,NULL,'0x664B322eb76f596d29d681e6ed39E36767647BEA',0,0,NULL,'{\"private_key\":\"0x3a7fb2df198cf285e593c7f7b463f456b808a6ed86302b9bad7f98cfd4290ecf\",\"kms_data_key_id\":0}',1634528911892,1634528911892,1),(18,2,'btc',NULL,NULL,NULL,'mw2tgpD8ZF32KRdUNy9yTDee5EHgvRD35R',0,0,NULL,'{\"private_key\":\"cR8phMcQua4r9Vy9WeDKfVpoYFd3aZkkPYoCWdTkVBFtoQLN5aCu\",\"kms_data_key_id\":0}',1634538134599,1634538134599,1),(19,2,'btc',NULL,NULL,NULL,'mu7bdxh8JS4zrqUEVvG99qbMy9QryX8Stn',0,0,NULL,'{\"private_key\":\"cP7SAmMgFk8EACNK1eXBgVLd5ScrRR8mGXpCRJBoeWLr4nmZ5F3D\",\"kms_data_key_id\":0}',1634538822291,1634538822291,1),(20,1,'eth',NULL,NULL,NULL,'0xD051E34BEC54519A450542eFb99DBDb115BA09Bc',0,0,NULL,'{\"private_key\":\"0x906d796a1d65a0fbf0820f36a2644dab7aa02fbd841935297bc72f455c378674\",\"kms_data_key_id\":0}',1634539216992,1634539216992,1),(21,1,'eth',NULL,NULL,NULL,'0x05cFb9A0eCa7426a0485353A12585164C2100AC5',0,0,NULL,'{\"private_key\":\"0xeb6466a9ff8ca514c8f604fd0fc7d567567600240dd032a02b2258df49040eb6\",\"kms_data_key_id\":0}',1634539238231,1634539238231,1),(22,2,'btc',NULL,NULL,NULL,'mguPCugxLpBMkQ4HwAGS2Qzf73aQhM8PjS',0,0,NULL,'{\"private_key\":\"cUJAGFK4JpDaFPcNk3Auo8x99KLRN3GQ8pXZ2J7yc6c1qKf6161K\",\"kms_data_key_id\":0}',1634539264708,1634539264708,1),(23,2,'btc',NULL,NULL,NULL,'mmsoRCmejwkfsmu1k5sNHNv6rcEHjDHAxY',0,0,NULL,'{\"private_key\":\"cRfXwUcU6kzXBbQc25DGboJmesyXWZpjAtsoyLWhwbBFztn1gj57\",\"kms_data_key_id\":0}',1634539268592,1634539268592,1),(24,2,'btc',NULL,NULL,NULL,'mtV9qryPyJvYkPL1rd1a3Jk7U7cMrHK1X7',0,0,NULL,'{\"private_key\":\"cRVkuh4d6MuES95nc3AzVHet3jGZjo97Zye5R2XoMRp84yD482Sf\",\"kms_data_key_id\":0}',1634539307555,1634539307555,1),(25,2,'btc',NULL,NULL,NULL,'mzmVK9m38KkZcrdzkhgH8UWfsm1ZowaU2n',0,0,NULL,'{\"private_key\":\"cUqgC6Gq9C1LUpGTdA96DAWf1ZXmuoRkrfx366L5P9jdAckLAEr5\",\"kms_data_key_id\":0}',1634542990211,1634542990211,1),(26,2,'btc',NULL,NULL,NULL,'mwgpYBLoWP685G6JZkYRa6xLKQYgHpwWnc',0,0,NULL,'{\"private_key\":\"cMifwHvUqf4PNj979m3r8mSaEnT6Wevxmr6EGJTahNrFSgsMu1ue\",\"kms_data_key_id\":0}',1634542992012,1634542992012,1),(27,2,'btc',NULL,NULL,NULL,'mkSibof4RwC9RZYmkCnz4vZoQ6SsNKUaoM',0,0,NULL,'{\"private_key\":\"cVJ8zo7NHBKcG9GTvKBFqKzs47PrkuKiiN3okmqNdShfAgqdayfN\",\"kms_data_key_id\":0}',1634543002335,1634543002335,1),(28,2,'btc',NULL,NULL,NULL,'mkgDx1YERHCTdido7X5G16mPDtex5sziCR',0,0,NULL,'{\"private_key\":\"cU1VghJidAkAxkCeXQAGHikGJqNpiUfmEt4j7cwyPG1EJXJZphGL\",\"kms_data_key_id\":0}',1634543043274,1634543043274,1),(29,1,'eth','wallet 1',NULL,NULL,'0xDc49Cfc94D7524dE92E7492a8e72b61b23158F03',0,0,NULL,'{\"private_key\":\"0xd2b4d9bf03843623f8cee3ac2aa31396d63f35baa006bc68c14b70da2c4a5d9f\",\"kms_data_key_id\":0}',1634544026572,1634544026572,1),(30,1,'eth','wallet A',NULL,NULL,'0x32e2bC0fb659a54BB84c9b64143A125A664af4bB',0,0,NULL,'{\"private_key\":\"0xe1e0e3c0b8e6557ea518c5e27ef4245b7bc8b3ddcdf2615bb3b578b34b3fe875\",\"kms_data_key_id\":0}',1634609543052,1634609543052,1),(31,1,'eth','wallet B',NULL,NULL,'0xE121D8098a172F9bfa054C4B39012E997890AE40',0,0,NULL,'{\"private_key\":\"0xc0a2c0b1d64ee082876284162cd1e522beb7c8962b9204330adc33ef16265625\",\"kms_data_key_id\":0}',1634609550424,1634609550424,1),(32,1,'eth','wallet C',NULL,NULL,'0x0a689FCf61c62531724f16BB46bcfAa730274e51',0,0,NULL,'{\"private_key\":\"0xaf9d1b59c5eb701496dbb19a156055b487109e560ecbba7c4c51d832650d9490\",\"kms_data_key_id\":0}',1634609563031,1634609563031,1),(33,1,'eth','ありがとうございます。',NULL,NULL,'0x40b12e3dD497cA5dFAce0e5623300b03af44BBb7',0,0,NULL,'{\"private_key\":\"0x1d82a0ced532903b0a8b5383833ab388e59134ab8ff1898f7e7edf40f0a4f553\",\"kms_data_key_id\":0}',1634609721381,1634609721381,1),(34,1,'eth','<p>title</p>',NULL,NULL,'0xa08844fBE45d54630fc5C7bEaf704a63640c9dDA',0,0,NULL,'{\"private_key\":\"0x1e9322352b2ddc0fa8419232df2b7f0d10385bbdae93d574f619235168eec855\",\"kms_data_key_id\":0}',1634609759900,1634609759900,1),(35,1,'eth','wallet\'s name',NULL,NULL,'0x73f295360933c4ce7fd27240aE9B8E0ba558bA8d',0,0,NULL,'{\"private_key\":\"0x562c9efa41beca9b224cd2b95741ed786c6f0eefd92bf3d41d36431fd600f5ee\",\"kms_data_key_id\":0}',1634609783632,1634699869200,1),(36,1,'eth','label :)',NULL,NULL,'0xF404F77F430772b97eD51c6cae74FC5A42C8fa94',0,0,NULL,'{\"private_key\":\"0x3195ef2cc4d69eada48721618d560c93f5e16ef6e995e0fd2dfcbcd843894be9\",\"kms_data_key_id\":0}',1634609834773,1634609834773,1),(37,2,'btc',NULL,NULL,NULL,'mxotNC2p79da1Z9HaAob8PT5c4D6tj3Png',0,0,NULL,'{\"private_key\":\"cRUgm7hJ9VhcgTVweiWq8je131bYskab7JwGK5j7xD7xpVcD9Muq\",\"kms_data_key_id\":0}',1634609998514,1634609998514,1),(38,2,'btc',NULL,NULL,NULL,'mr3uGcDbDRCVxE32RJhqGpZgroQVM5cH8N',0,0,NULL,'{\"private_key\":\"cNLGLyCCvUNx6yWLdizdGWRZJnZ69N65KD7oMDEaBmUUyF1SzMsA\",\"kms_data_key_id\":0}',1634610063164,1634610063164,1),(39,2,'btc',NULL,NULL,NULL,'mzmtVKg33aoaHnmzAv3ABNnJPsHVirqdMY',0,0,NULL,'{\"private_key\":\"cVvTjMvGA21rd6kQMeJqrGSz224KHM5bHr8dMPc22y7kzQ4xmqef\",\"kms_data_key_id\":0}',1634610125704,1634610125704,1),(40,2,'btc',NULL,NULL,NULL,'moLhvte3s6w2nz7JaqTJAMPLFCQaaxovaf',0,0,NULL,'{\"private_key\":\"cPpv2GVD4ezNGDNKRDJtoaGeWiZNjiHC7TgJCakajzzHnyc8qWHz\",\"kms_data_key_id\":0}',1634610142357,1634610142357,1),(41,2,'btc',NULL,NULL,NULL,'mvu3UyqKtNwPDPbkDTtV3VpEeMX7B1h4gy',0,0,NULL,'{\"private_key\":\"cQSoD5nXDg8mCb9ZfDv7kxvvmwgzvHUyLwZMRwwcqKtovmL615i6\",\"kms_data_key_id\":0}',1634610211960,1634610211960,1),(42,1,'eth',NULL,NULL,NULL,'0x7969f6a1d1898c227959Af547082639663d0B600',0,0,NULL,'{\"private_key\":\"0x9135d2d9528726751efd5badb7fd265ecfb8c01b5bb530f2a9b2dd844b8dbb09\",\"kms_data_key_id\":0}',1634610232799,1634610232799,1),(43,2,'btc',NULL,NULL,NULL,'mi439akYXT6qUDQLAL1wRzRKdbHftvzNeV',0,0,NULL,'{\"private_key\":\"cSvaTGTBRnPBgY3nKdvYrq6g99kJJm81MCupAbifrCaQczdUMeSv\",\"kms_data_key_id\":0}',1634610267009,1634610267009,1),(44,2,'btc',NULL,NULL,NULL,'mm5WPY4azXopYfJ1hzED9HvE5gHgmNGrAP',0,0,NULL,'{\"private_key\":\"cVtAx6dFGFcRx9vp8uXtgyyTk5uaEuhVvbWDg5o94nj4jojkZDbo\",\"kms_data_key_id\":0}',1634610506566,1634610506566,1),(45,2,'btc',NULL,NULL,NULL,'mxNiTVwMLJBCJiNNpMku9mf8K75ddJgm21',0,0,NULL,'{\"private_key\":\"cU6SMeihxmUvHGR6uVmzFS5X4gC3yxmmNkm8bHTU5bWZjnGpL2mk\",\"kms_data_key_id\":0}',1634610568651,1634610568651,1),(46,2,'btc','testing for btc',NULL,NULL,'mrViqWDXeAT3qhRgkQ4FWQExf8gL27MS9D',0,0,NULL,'{\"private_key\":\"cTrLginHJwumWbKJPwqH7MjsuWFoXQKX1y7J77bj1FFWBDbJbHZs\",\"kms_data_key_id\":0}',1634610636675,1634610636675,1),(47,2,'btc','tesst',NULL,NULL,'mz2mwoXTrgEPSMTK42u7xKBPr263wjEBp4',0,0,NULL,'{\"private_key\":\"cTA61cCZ99CTVoowRAFBtezt2RzNgxwyJtDy2Z6ctoCDqq1mK45Y\",\"kms_data_key_id\":0}',1634610797666,1634610797666,1),(48,2,'btc',NULL,NULL,NULL,'mkxpXdDdJHKh3dFonzYfkERrrgcrQy3TN8',0,0,NULL,'{\"private_key\":\"cR1yU3JoGqdq3UvssDTffut6MJDu3JYvf2kqNNUqJaWCCRDwXW3A\",\"kms_data_key_id\":0}',1634610816639,1634610816639,1),(49,2,'btc',NULL,NULL,NULL,'mvtK1VD7VHCwD1S6ewwkbGkaK4Cw1KkQZb',0,0,NULL,'{\"private_key\":\"cMwv76JuPrAfKwn5ABEGjvftMaapkyWcc9J3aFKjDSMaVGyCDFBh\",\"kms_data_key_id\":0}',1634610870922,1634610870922,1),(50,2,'btc',NULL,NULL,NULL,'mmBUe36DqpkbfqAsEjtmfYLnKM1t2yBGEi',0,0,NULL,'{\"private_key\":\"cPtE2f7rbcVSQkepiZLBfy7fYagRLQVtMfNiJk78gKYKErkLv47Q\",\"kms_data_key_id\":0}',1634610880272,1634610880272,1),(51,2,'btc',NULL,NULL,NULL,'mpQsdSW9gJTBSpDKTQmioeGwYKuZoDzZgA',0,0,NULL,'{\"private_key\":\"cVsx8AubKYcTAgsxPcs3BzUhKVUmrs94K7MLGHTANmybVLbqsRUK\",\"kms_data_key_id\":0}',1634610882129,1634610882129,1),(52,1,'eth','あ',NULL,NULL,'0xBdBfAabDC03c4DA06dDe1be18bb5f5501B3C269F',0,0,NULL,'{\"private_key\":\"0xc1b8b069f8664ebd94c1a1f15b40a417cd0b15b2e6fd438e78d71000c374401c\",\"kms_data_key_id\":0}',1634610976458,1634610976458,1),(53,2,'btc','abc',NULL,NULL,'mvsT4J6tpWTVfkxqAH9HhPoJrso9GcB4vf',0,0,NULL,'{\"private_key\":\"cTCqupHXjnX5qsEfMBeBjeJM5amYyqMMaW2kkPni9qZ61nnXSC9X\",\"kms_data_key_id\":0}',1634611151608,1634611151608,1),(54,2,'btc','BTC coin',NULL,NULL,'n4LLbUwnaZUfJX2TSKgSvFGHVaeQ6ATWHZ',0,0,NULL,'{\"private_key\":\"cVNTcxUXTTpiC6dx4d5BU6myaqvJUm9GupVR26qpzUSZXTw7hizU\",\"kms_data_key_id\":0}',1634612127518,1634612127518,1),(55,3,'xrp',NULL,NULL,NULL,'rE26C8twBWFk4w8sU6Quu458GT1V7Eynoc',0,0,NULL,'{\"private_key\":\"snRKV496upieuvPQKu9MANx7pH5b8\",\"kms_data_key_id\":0}',1634636340627,1634636340627,1),(56,1,'eth','ETH\'s Wallet',NULL,NULL,'0xc88dDA4CD69484977bD79b98E9Acd94D7FDD1Ee0',0,0,NULL,'{\"private_key\":\"0xafaeb940ddb3f4afdbbd1e97dd61f6ece45b2b26b77b6b1b08f9aa55c862fa2a\",\"kms_data_key_id\":0}',1634636616524,1634636616524,1),(57,1,'eth','wallet 2',NULL,NULL,'0x5DF485521e34BCfc790602542a82a2d1dDB003B0',0,0,NULL,'{\"private_key\":\"0xab500992f0a647c1b4a5b2fc0dc1d239dbd0931a6399f179be3abc7f7cafc420\",\"kms_data_key_id\":0}',1634636728723,1634636728723,1),(58,1,'eth','string',NULL,NULL,'0x43fA3cc6ea439728a9b18508cbB3643981Cc3239',0,0,NULL,'{\"private_key\":\"0x2b49f9cc7b936495036ba7666167c6648ed41ffdc7195fb267a06d3de807ff44\",\"kms_data_key_id\":0}',1634636907516,1634636907516,1),(59,2,'btc','string',NULL,NULL,'mu1gxQMGybGyi35rAfgADG6aZqt8eGgG8r',0,0,NULL,'{\"private_key\":\"cSMxG4eFhz3YnAqCfvMkEcwZR4AkDCjj68yg2hHL9LH94Zvq4sTU\",\"kms_data_key_id\":0}',1634637082093,1634637082093,1),(60,2,'btc','testing',NULL,NULL,'mn4FinJrs5fwKSX8hHhj1X2htqCPbEVfW8',0,0,NULL,'{\"private_key\":\"cU5aKrEFYZWhGbfaPFUA9C5SHHVwCkPp9k9PvPE1LGDR1fSVuAqd\",\"kms_data_key_id\":0}',1634643496988,1634797953636,1),(61,3,'xrp','test xrp',NULL,NULL,'F1aCL3BZ3ySVRfGw7XjdY7c51G4BhPjZBRX1xvqXjSRn',0,0,NULL,'{\"private_key\":{\"0\":95,\"1\":122,\"2\":18,\"3\":235,\"4\":41,\"5\":59,\"6\":247,\"7\":234,\"8\":70,\"9\":19,\"10\":158,\"11\":36,\"12\":85,\"13\":22,\"14\":38,\"15\":200,\"16\":232,\"17\":231,\"18\":209,\"19\":194,\"20\":22,\"21\":254,\"22\":14,\"23\":132,\"24\":201,\"25\":171,\"26\":57,\"27\":108,\"28\":136,\"29\":25,\"30\":144,\"31\":49,\"32\":208,\"33\":41,\"34\":38,\"35\":169,\"36\":240,\"37\":163,\"38\":115,\"39\":110,\"40\":121,\"41\":235,\"42\":182,\"43\":218,\"44\":149,\"45\":69,\"46\":14,\"47\":102,\"48\":104,\"49\":221,\"50\":48,\"51\":41,\"52\":51,\"53\":75,\"54\":248,\"55\":111,\"56\":233,\"57\":16,\"58\":169,\"59\":152,\"60\":117,\"61\":12,\"62\":55,\"63\":81},\"kms_data_key_id\":0}',1634643504934,1634717257167,1),(62,4,'sol','string',NULL,NULL,'rNfCvsEotwYin8DJXpG8jrc5bRzVNCTdgv',0,0,NULL,'{\"private_key\":\"sanXzpE7eB9DV4cnS3EL91cM1gfTH\",\"kms_data_key_id\":0}',1634643510521,1634643510521,1),(63,1,'eth','string',NULL,NULL,'0xed26523b0393e94d67B5EfEeb71C42d679B9f6E1',0,0,NULL,'{\"private_key\":\"0x73fc247e91cced136e84ee10537ed55a479f67ea7da3f3a3adfa5c17c07d0679\",\"kms_data_key_id\":0}',1634643728947,1634643728947,1),(64,1,'eth','string',NULL,NULL,'0x06e187DFb28d19e5df0c8bBB0C8880DB503d8B76',0,0,NULL,'{\"private_key\":\"0xbab5e4f8a4158bab5dce67a11cc4eba0a69c9cf4304b2de31b91589f60bb21a6\",\"kms_data_key_id\":0}',1634643732070,1634643732070,1),(65,1,'eth','123abc',NULL,NULL,'0x6B69D99aceA9749C7D1FC9BaA46Ede2289E55562',0,0,NULL,'{\"private_key\":\"0xc2db8b8c94b5eb463ff112aa7600f62c8ff2fcf34b9071c78e10b433be208c3e\",\"kms_data_key_id\":0}',1634643740955,1634699687757,1),(66,1,'eth','test update',NULL,NULL,'0xe71759453433a60844683aa6D95Bd65D62C0931A',0,0,NULL,'{\"private_key\":\"0xaee8d38f773bb136ab1e5e84d6aae161669da7e867130fcabc60cea3e5498ba4\",\"kms_data_key_id\":0}',1634669843154,1634697016530,1),(67,1,'eth','eth123',NULL,NULL,'0x2D94C6C349a882612FdFF454A84b9e296e9254cd',0,0,NULL,'{\"private_key\":\"0x16e4cd66d8af34cd010b5e0b278274e304079922e7710809dd50ac9426b16cdc\",\"kms_data_key_id\":0}',1634696596993,1634696975710,1),(68,1,'eth','wallet\'s name',NULL,NULL,'0x93e7aA42E74DCbeC148e21d18048e46899Da9b9C',0,0,NULL,'{\"private_key\":\"0x27e0a4fe3e68915c53b7c210ac8173e8d7e49c5c968be02566f722db211afdac\",\"kms_data_key_id\":0}',1634697656410,1634700008060,1),(69,1,'eth','',NULL,NULL,'0x4B8F66Fa36f724E176B4F689f6334835D9C0C9BB',0,0,NULL,'{\"private_key\":\"0x93fd63aaf7588c63e7b2a888d6b30fc3954c640c28cf978a9636d3bd5d89bb4d\",\"kms_data_key_id\":0}',1634697963623,1634697963623,1),(70,2,'btc','This is a testing This is a testing This is a testing This is a testing This is a testing This is a testing This is a testing This is a testing This is a testing This is a testing This is a testing This is a testing This is a testing This is a testin',NULL,NULL,'mjqyBnFw76hHDNTPPxpY52jPUoeQvUAqkG',0,0,NULL,'{\"private_key\":\"cNC79TXFEx1cGMhyLDHkcVyFfR3WzTBLCs7M671K4TFt6cfrES1R\",\"kms_data_key_id\":0}',1634702132832,1634712794764,1),(71,2,'btc','Testing',NULL,NULL,'mgkPupgARhNE2ib9yArBfswFv1KR4dMJfL',0,0,NULL,'{\"private_key\":\"cQT32r43T35vfPgiGAj3Zzybqmcpag73357H6XBb6Jo6USHM7YkL\",\"kms_data_key_id\":0}',1634702478324,1634726800289,1),(72,4,'sol','Test for solana',NULL,NULL,'AVqQWznmi6P3GTDuPvoywTRidrtsZxCrK578FgmH1UDm',0,0,NULL,'{\"private_key\":{\"0\":14,\"1\":127,\"2\":157,\"3\":74,\"4\":72,\"5\":114,\"6\":193,\"7\":8,\"8\":116,\"9\":204,\"10\":45,\"11\":136,\"12\":210,\"13\":238,\"14\":75,\"15\":69,\"16\":247,\"17\":2,\"18\":154,\"19\":18,\"20\":35,\"21\":57,\"22\":63,\"23\":68,\"24\":1,\"25\":43,\"26\":219,\"27\":25,\"28\":255,\"29\":236,\"30\":34,\"31\":160,\"32\":141,\"33\":28,\"34\":45,\"35\":94,\"36\":226,\"37\":253,\"38\":148,\"39\":160,\"40\":198,\"41\":58,\"42\":193,\"43\":97,\"44\":193,\"45\":226,\"46\":214,\"47\":154,\"48\":166,\"49\":39,\"50\":23,\"51\":58,\"52\":247,\"53\":9,\"54\":223,\"55\":48,\"56\":177,\"57\":189,\"58\":34,\"59\":131,\"60\":183,\"61\":182,\"62\":231,\"63\":240},\"kms_data_key_id\":0}',1634702549679,1634702549679,1),(73,1,'eth','this is usdt\'s wallet',NULL,NULL,'0x3E2A4Cb840Fc0593d0B1605FA83De52b3cf17a80',0,0,NULL,'{\"private_key\":\"0xce0d67289d0b4d998f6de22a8f8ea277abf2588880ec3c14d31f9dfe9d8bf4f8\",\"kms_data_key_id\":0}',1634704900204,1634714397814,1),(74,1,'eth','test update',NULL,NULL,'0xb9ac6E3Dc9ADB34329a6132D0652428d459DF3Dd',0,0,NULL,'{\"private_key\":\"0xdec15c1ca756a05b7511c3b8415dd8c721a8b91ac9b7793f97a22a5ec15a2244\",\"kms_data_key_id\":0}',1634704957320,1634739776453,1),(75,4,'sol','solana testing',NULL,NULL,'EUrFdVATaCaZAGJTCvd8LW3MLdogHJuorFdwGBiGrXYr',0,0,NULL,'{\"private_key\":{\"0\":20,\"1\":126,\"2\":124,\"3\":20,\"4\":128,\"5\":236,\"6\":17,\"7\":243,\"8\":190,\"9\":34,\"10\":159,\"11\":9,\"12\":190,\"13\":180,\"14\":162,\"15\":183,\"16\":113,\"17\":38,\"18\":235,\"19\":250,\"20\":243,\"21\":107,\"22\":23,\"23\":19,\"24\":187,\"25\":161,\"26\":202,\"27\":238,\"28\":88,\"29\":212,\"30\":50,\"31\":184,\"32\":200,\"33\":74,\"34\":77,\"35\":50,\"36\":201,\"37\":48,\"38\":181,\"39\":82,\"40\":49,\"41\":5,\"42\":144,\"43\":130,\"44\":134,\"45\":193,\"46\":254,\"47\":35,\"48\":29,\"49\":179,\"50\":138,\"51\":249,\"52\":45,\"53\":207,\"54\":42,\"55\":129,\"56\":126,\"57\":212,\"58\":166,\"59\":92,\"60\":70,\"61\":72,\"62\":7,\"63\":167},\"kms_data_key_id\":0}',1634705953414,1634705953414,1),(76,2,'btc',NULL,NULL,NULL,'miHvv8xcep9zYwsbm3dwZdXJXxjZBwKF17',0,0,NULL,'{\"private_key\":\"cQCNr46b4MymjMDi34PSWkuoKHMnibriQPA29VjezsK6xVwjDJ2B\",\"kms_data_key_id\":0}',1634711073683,1634711073683,1),(77,2,'btc','solana testing',NULL,NULL,'mmfMMC98foUwkWqLDzrqowgFMTgDP4tomH',0,0,NULL,'{\"private_key\":\"cSsZbbXrD1XYucNGA7Ld3W8jDkouJCTKzvVQK4c89urWqZ9o2m7y\",\"kms_data_key_id\":0}',1634711212416,1634711212416,1),(78,2,'btc','This is a testing',NULL,NULL,'n2jdJpBBVCAQhshxhG7sL6KB71Rk8Tz5zU',0,0,NULL,'{\"private_key\":\"cMhmspLCurDF8B9HMfDQKekpJ1fkAL9RYBwmZiXknGMQ5dSgJ4LJ\",\"kms_data_key_id\":0}',1634712535105,1634726355077,1),(79,1,'eth','',NULL,NULL,'0x98Ef600427aDe99CeeC4460D6feea1CEb72D264d',0,0,NULL,'{\"private_key\":\"0xf78e031745239933ff9984283a9aa8c12967115e1f6968e167c71d575af51658\",\"kms_data_key_id\":0}',1634713337735,1634713337735,1),(80,2,'btc','   32424   23423    ',NULL,NULL,'mgaRMgMcMBrDVMSJjuPrnGnSSoCsgp3PQQ',0,0,NULL,'{\"private_key\":\"cQG4a6oSEPurjRFHWJraxx6xMZ91wBPQ8SDAMTs3vZagjnpghysn\",\"kms_data_key_id\":0}',1634713595902,1634713595902,1),(81,3,'xrp','xrp 1',NULL,NULL,'rLZpraknDMNbBH8RRHJsYcE5byFwbHqt3o',0,0,NULL,'{\"private_key\":\"shTyb95pPtaggLtAutjwF7gGzA5FY\",\"kms_data_key_id\":0}',1634713678612,1634717205909,1),(82,1,'eth','usdc wallet',NULL,NULL,'0x43Cf41ee3815698156c7b2d15A944b44E17a48f3',0,0,NULL,'{\"private_key\":\"0x6793c3bc11cba3b844ff7a673279ce97da5bc1db6b085844cca8db67a0d863ad\",\"kms_data_key_id\":0}',1634713895420,1634713895420,1),(83,3,'xrp','xrp testing',NULL,NULL,'r4Ld8xbYAJGyKc5N3ttSNNG8XDTDeMqvCY',0,0,NULL,'{\"private_key\":\"snx37Dbgeh5LUG2297qe9oD4TmgUf\",\"kms_data_key_id\":0}',1634713947437,1634713947437,1),(84,2,'btc',NULL,NULL,NULL,'mtQUZMcw8iDtwjADhTGdUVm2MBSNqCWy9Y',0,0,NULL,'{\"private_key\":\"cS11GaLKBqwTx5EKf5Y7ukkMpG4thRMdGmwX1uvao2SSWvhBXQRL\",\"kms_data_key_id\":0}',1634714397903,1634806926801,1),(85,3,'xrp','test update',NULL,NULL,'rnu9dMp9fh3oaWXQSw8iSucbGkd3M9nzek',0,0,NULL,'{\"private_key\":\"ssmCwKofis9mPubWj7e3w3Rtu2V3h\",\"kms_data_key_id\":0}',1634717363987,1634740053637,1),(86,3,'xrp','test xrp',NULL,NULL,'r4Au1N71aT7MyZM7Bk6zZb74w2FHvA7WtK',0,0,NULL,'{\"private_key\":\"ssaAMH2Ugbzb56jFTHqNzVqmt3hgQ\",\"kms_data_key_id\":0}',1634721402398,1634739679938,1),(87,2,'btc',NULL,NULL,NULL,'mtDtGZL89Vd7YUG39zrpW9EKM5J47ho7Nd',0,0,NULL,'{\"private_key\":\"cPVvKPAeFWznpV4tN65chi5fzSoVend4prVvrrbpL7DXhKEdvHow\",\"kms_data_key_id\":0}',1634721960180,1634721960180,1),(88,2,'btc','Testing',NULL,NULL,'mjJYYK8vVK8idQpeZQZVt91JX27yySaDR4',0,0,NULL,'{\"private_key\":\"cRpXXqtaQLpaNwX1Pnx7idTjrzekDd2KQGGDZpGT52Z7MiWtvgyN\",\"kms_data_key_id\":0}',1634722411632,1634726875868,1),(89,4,'sol','nnn   iiii  o',NULL,NULL,'4RpLcZNwUQno58z26Vo97z74ghXiuSzdQPgA8AQ4XdEp',0,0,NULL,'{\"private_key\":{\"0\":59,\"1\":118,\"2\":1,\"3\":64,\"4\":192,\"5\":9,\"6\":208,\"7\":76,\"8\":102,\"9\":110,\"10\":22,\"11\":192,\"12\":202,\"13\":33,\"14\":55,\"15\":45,\"16\":74,\"17\":74,\"18\":232,\"19\":217,\"20\":83,\"21\":48,\"22\":114,\"23\":222,\"24\":223,\"25\":172,\"26\":255,\"27\":206,\"28\":194,\"29\":62,\"30\":91,\"31\":130,\"32\":50,\"33\":238,\"34\":134,\"35\":143,\"36\":24,\"37\":185,\"38\":112,\"39\":212,\"40\":64,\"41\":197,\"42\":161,\"43\":97,\"44\":220,\"45\":114,\"46\":25,\"47\":6,\"48\":176,\"49\":28,\"50\":181,\"51\":37,\"52\":158,\"53\":195,\"54\":113,\"55\":230,\"56\":90,\"57\":255,\"58\":75,\"59\":57,\"60\":126,\"61\":144,\"62\":22,\"63\":49},\"kms_data_key_id\":0}',1634723226598,1634806070219,1),(90,1,'eth','              string',NULL,NULL,'0x90b6347484A17e45A4B6b60f1845e2502340Ac1B',0,0,NULL,'{\"private_key\":\"0x5dc7b55c3bde1cc6d61eb67abb4c6da4a6436ee0383e1f28ab5baeb4c1ea2e81\",\"kms_data_key_id\":0}',1634750009147,1634750009147,1),(91,4,'sol',NULL,NULL,NULL,'939BRcWx4wsAKAjqvUY7112GJTkp9HEVPykCoRBHRyzt',0,0,NULL,'{\"private_key\":{\"0\":137,\"1\":190,\"2\":103,\"3\":193,\"4\":211,\"5\":116,\"6\":204,\"7\":91,\"8\":124,\"9\":234,\"10\":99,\"11\":184,\"12\":182,\"13\":73,\"14\":138,\"15\":233,\"16\":111,\"17\":153,\"18\":8,\"19\":74,\"20\":11,\"21\":203,\"22\":80,\"23\":100,\"24\":253,\"25\":148,\"26\":189,\"27\":204,\"28\":143,\"29\":133,\"30\":250,\"31\":110,\"32\":119,\"33\":105,\"34\":231,\"35\":231,\"36\":181,\"37\":254,\"38\":233,\"39\":209,\"40\":199,\"41\":198,\"42\":71,\"43\":33,\"44\":108,\"45\":66,\"46\":158,\"47\":11,\"48\":199,\"49\":237,\"50\":204,\"51\":238,\"52\":14,\"53\":243,\"54\":42,\"55\":100,\"56\":213,\"57\":167,\"58\":103,\"59\":141,\"60\":251,\"61\":134,\"62\":27,\"63\":253},\"kms_data_key_id\":0}',1634783655180,1634783655180,1),(92,1,'eth','string',NULL,NULL,'0xF2B17a01A704517e7F0350Bd6a10ea5cF265D5cd',0,0,NULL,'{\"private_key\":\"0x668111715f2537a90ff5f2544723e9ad2271816f201001551549c0b4b96f87ea\",\"kms_data_key_id\":0}',1634788084119,1634788084119,1),(93,4,'sol','   ',NULL,NULL,'3x1kpYVXuA8ZREZGqz5XR4jhNjktNTENSVmeTxEgL676',0,0,NULL,'{\"private_key\":{\"0\":12,\"1\":181,\"2\":168,\"3\":202,\"4\":40,\"5\":213,\"6\":127,\"7\":142,\"8\":96,\"9\":30,\"10\":227,\"11\":254,\"12\":243,\"13\":92,\"14\":248,\"15\":115,\"16\":105,\"17\":140,\"18\":22,\"19\":25,\"20\":19,\"21\":90,\"22\":230,\"23\":189,\"24\":181,\"25\":103,\"26\":209,\"27\":60,\"28\":112,\"29\":199,\"30\":10,\"31\":123,\"32\":43,\"33\":207,\"34\":44,\"35\":48,\"36\":79,\"37\":187,\"38\":177,\"39\":59,\"40\":7,\"41\":9,\"42\":202,\"43\":49,\"44\":190,\"45\":80,\"46\":130,\"47\":32,\"48\":36,\"49\":76,\"50\":248,\"51\":253,\"52\":2,\"53\":17,\"54\":114,\"55\":129,\"56\":169,\"57\":139,\"58\":210,\"59\":242,\"60\":89,\"61\":180,\"62\":248,\"63\":93},\"kms_data_key_id\":0}',1634788304713,1634788304713,1),(94,4,'sol','%%',NULL,NULL,'HuAB6WKUjnVdRcSXQJacr7NT5dtxLdQmacUeNfyKf6Jq',0,0,NULL,'{\"private_key\":{\"0\":186,\"1\":119,\"2\":17,\"3\":54,\"4\":253,\"5\":158,\"6\":165,\"7\":230,\"8\":91,\"9\":152,\"10\":25,\"11\":15,\"12\":4,\"13\":106,\"14\":166,\"15\":177,\"16\":18,\"17\":164,\"18\":44,\"19\":107,\"20\":49,\"21\":208,\"22\":37,\"23\":142,\"24\":57,\"25\":37,\"26\":64,\"27\":240,\"28\":121,\"29\":41,\"30\":12,\"31\":198,\"32\":251,\"33\":23,\"34\":144,\"35\":231,\"36\":10,\"37\":14,\"38\":252,\"39\":146,\"40\":122,\"41\":61,\"42\":42,\"43\":57,\"44\":117,\"45\":86,\"46\":50,\"47\":33,\"48\":82,\"49\":50,\"50\":4,\"51\":139,\"52\":84,\"53\":152,\"54\":202,\"55\":21,\"56\":242,\"57\":198,\"58\":129,\"59\":63,\"60\":84,\"61\":96,\"62\":78,\"63\":206},\"kms_data_key_id\":0}',1634788385189,1634788385189,1),(95,4,'sol','',NULL,NULL,'6Jih1YvTbbFkHQS6oU3YC95ZZPKiphXhUKG8jzzGZPdQ',0,0,NULL,'{\"private_key\":{\"0\":217,\"1\":171,\"2\":202,\"3\":81,\"4\":222,\"5\":173,\"6\":204,\"7\":24,\"8\":143,\"9\":171,\"10\":156,\"11\":4,\"12\":251,\"13\":184,\"14\":136,\"15\":120,\"16\":205,\"17\":217,\"18\":190,\"19\":59,\"20\":85,\"21\":14,\"22\":167,\"23\":134,\"24\":41,\"25\":208,\"26\":71,\"27\":122,\"28\":248,\"29\":227,\"30\":38,\"31\":217,\"32\":78,\"33\":212,\"34\":114,\"35\":124,\"36\":164,\"37\":204,\"38\":66,\"39\":74,\"40\":18,\"41\":224,\"42\":185,\"43\":120,\"44\":98,\"45\":135,\"46\":49,\"47\":7,\"48\":138,\"49\":115,\"50\":202,\"51\":67,\"52\":53,\"53\":180,\"54\":32,\"55\":122,\"56\":98,\"57\":210,\"58\":252,\"59\":252,\"60\":100,\"61\":108,\"62\":13,\"63\":39},\"kms_data_key_id\":0}',1634788494786,1634788494786,1),(96,4,'sol','````',NULL,NULL,'2uwbhzxoFua4kqb5977bd3KC8AybKBTwRAocUrT5kMmW',0,0,NULL,'{\"private_key\":{\"0\":113,\"1\":181,\"2\":87,\"3\":171,\"4\":20,\"5\":196,\"6\":7,\"7\":176,\"8\":195,\"9\":59,\"10\":18,\"11\":122,\"12\":52,\"13\":116,\"14\":226,\"15\":62,\"16\":140,\"17\":85,\"18\":166,\"19\":215,\"20\":183,\"21\":216,\"22\":150,\"23\":12,\"24\":209,\"25\":224,\"26\":136,\"27\":220,\"28\":174,\"29\":246,\"30\":244,\"31\":215,\"32\":28,\"33\":107,\"34\":159,\"35\":164,\"36\":60,\"37\":189,\"38\":234,\"39\":145,\"40\":114,\"41\":95,\"42\":189,\"43\":20,\"44\":145,\"45\":113,\"46\":196,\"47\":97,\"48\":228,\"49\":80,\"50\":33,\"51\":73,\"52\":21,\"53\":128,\"54\":73,\"55\":138,\"56\":243,\"57\":161,\"58\":81,\"59\":165,\"60\":130,\"61\":202,\"62\":28,\"63\":221},\"kms_data_key_id\":0}',1634788543825,1634788543825,1),(97,4,'sol','~!@#$%^&*()_+|',NULL,NULL,'4WXayCqPEUNCyCLoJ4Tj7Qrjib6mMnkPaarmQ894QP5j',0,0,NULL,'{\"private_key\":{\"0\":116,\"1\":173,\"2\":128,\"3\":227,\"4\":73,\"5\":111,\"6\":127,\"7\":122,\"8\":35,\"9\":138,\"10\":121,\"11\":56,\"12\":169,\"13\":134,\"14\":173,\"15\":140,\"16\":61,\"17\":204,\"18\":232,\"19\":247,\"20\":26,\"21\":209,\"22\":20,\"23\":185,\"24\":51,\"25\":94,\"26\":213,\"27\":210,\"28\":239,\"29\":191,\"30\":91,\"31\":84,\"32\":52,\"33\":35,\"34\":124,\"35\":209,\"36\":149,\"37\":8,\"38\":155,\"39\":251,\"40\":133,\"41\":124,\"42\":205,\"43\":243,\"44\":226,\"45\":51,\"46\":220,\"47\":148,\"48\":205,\"49\":161,\"50\":198,\"51\":95,\"52\":182,\"53\":132,\"54\":123,\"55\":112,\"56\":232,\"57\":93,\"58\":195,\"59\":150,\"60\":254,\"61\":117,\"62\":127,\"63\":50},\"kms_data_key_id\":0}',1634798968238,1634798968238,1),(98,1,'eth','Wallet Name',NULL,NULL,'0xe7491FC2ABE8ac8081B674DFd7EcF3655ae8FF39',0,0,NULL,'{\"private_key\":\"0x79e56848ce6f9418053e22c6a4b013ad50b8d000142fcb25e32d4067d956e56c\",\"kms_data_key_id\":0}',1634804437248,1634807124695,3),(99,4,'sol',NULL,NULL,NULL,'5tWYX8oFD4kgivdngrmygNTHBVaHxHfvFVjuoTZNtA5t',0,0,NULL,'{\"private_key\":{\"0\":36,\"1\":107,\"2\":250,\"3\":169,\"4\":161,\"5\":16,\"6\":51,\"7\":201,\"8\":72,\"9\":60,\"10\":63,\"11\":177,\"12\":74,\"13\":109,\"14\":223,\"15\":168,\"16\":141,\"17\":23,\"18\":189,\"19\":100,\"20\":164,\"21\":111,\"22\":60,\"23\":243,\"24\":6,\"25\":64,\"26\":205,\"27\":34,\"28\":122,\"29\":133,\"30\":235,\"31\":218,\"32\":72,\"33\":160,\"34\":198,\"35\":26,\"36\":148,\"37\":241,\"38\":210,\"39\":159,\"40\":197,\"41\":14,\"42\":116,\"43\":70,\"44\":79,\"45\":100,\"46\":241,\"47\":77,\"48\":9,\"49\":157,\"50\":41,\"51\":201,\"52\":132,\"53\":137,\"54\":239,\"55\":57,\"56\":139,\"57\":84,\"58\":180,\"59\":54,\"60\":44,\"61\":250,\"62\":33,\"63\":39},\"kms_data_key_id\":0}',1634804640307,1634804640307,1),(100,1,'eth','usdt testing',NULL,NULL,'0xd7d5D58AD6C45c51CC373467b8F0E8E7804Dcb89',0,0,NULL,'{\"private_key\":\"0x17cd87bbc4a43e9caebbfc6f40088891e6a84155611dca09203ff53acdf62065\",\"kms_data_key_id\":0}',1634805035619,1634805035619,3),(101,1,'eth','usdc 1',NULL,NULL,'0x0e98C7ee955f0528aEc5340137cf9E2da0535549',0,0,NULL,'{\"private_key\":\"0xe450e8c1336c63ecc6ba196a4b1fe6665f63e30ad2b4cfa485c60b9b0ea178a0\",\"kms_data_key_id\":0}',1634805804399,1634805804399,3),(102,1,'eth','usdc thcshvhsvjerugkkgjeivdfhboijgegjvhrvhdfvedoe2643486496-0-057',NULL,NULL,'0x3dC8FbDd03cc648A44aF45498ABa54b9992Fcf2f',0,0,NULL,'{\"private_key\":\"0xb7d7e2fd3444fd5dd7843d5850835664306370baf827a7f612f6f370bc1f0077\",\"kms_data_key_id\":0}',1634805820377,1634805820377,3),(103,4,'sol',NULL,NULL,NULL,'CNtBAhEeXr8Y4ry7t6bWmcgpb4L2DoTLiXz81V31pjf4',0,0,NULL,'{\"private_key\":{\"0\":182,\"1\":64,\"2\":145,\"3\":33,\"4\":21,\"5\":108,\"6\":57,\"7\":139,\"8\":30,\"9\":147,\"10\":68,\"11\":197,\"12\":28,\"13\":152,\"14\":76,\"15\":163,\"16\":77,\"17\":218,\"18\":110,\"19\":242,\"20\":6,\"21\":241,\"22\":80,\"23\":158,\"24\":152,\"25\":66,\"26\":253,\"27\":161,\"28\":254,\"29\":80,\"30\":56,\"31\":232,\"32\":169,\"33\":11,\"34\":158,\"35\":5,\"36\":128,\"37\":28,\"38\":87,\"39\":130,\"40\":218,\"41\":138,\"42\":103,\"43\":4,\"44\":51,\"45\":201,\"46\":217,\"47\":116,\"48\":3,\"49\":234,\"50\":168,\"51\":210,\"52\":132,\"53\":85,\"54\":247,\"55\":222,\"56\":85,\"57\":211,\"58\":30,\"59\":2,\"60\":210,\"61\":161,\"62\":217,\"63\":31},\"kms_data_key_id\":0}',1634806084531,1634806833805,1),(104,1,'eth','nnn   iiii  o',NULL,NULL,'0xda1Ae7dB326cF9Ed3a54a95c7b38238bd84EBE61',0,0,NULL,'{\"private_key\":\"0x126cc1bb89e9cf408bd4a67616324cf2a1df901eea6940ac0ce99f782feff40a\",\"kms_data_key_id\":0}',1634806126610,1634806126610,1),(105,3,'xrp','test',NULL,NULL,'rLBanbofWrtmE7UNB3xSHjioZurvCdQBon',0,0,NULL,'{\"private_key\":\"shekNZ2X7MhvTxPw8jFQAmtTpdrTD\",\"kms_data_key_id\":0}',1634806202073,1634806739139,1),(106,1,'eth','Wallet Name',NULL,NULL,'0x3f2d44ce96D0dc84085a2AE0d8B8d077dd099B34',0,0,NULL,'{\"private_key\":\"0xd85d28881e3ba433a46c01032e6c63925300745b7c5fcd609845d20b4faedd21\",\"kms_data_key_id\":0}',1634806983722,1634807020194,3),(107,1,'eth','this is usdc\'s wallet',NULL,NULL,'0x18eaD3335ff3AA805d3DfE5047f8ACde4317aCbe',0,0,NULL,'{\"private_key\":\"0x262e138208a950897f2fe56f59132eb196c3f053aa1ab70ee097b772267b0bb2\",\"kms_data_key_id\":0}',1634807241195,1634807241195,3),(108,1,'eth','this is usdc\'s wallet',NULL,NULL,'0x3DADdF18DDEd3FbA343A7401b8661f9e9C800752',0,0,NULL,'{\"private_key\":\"0x1a710e79a7473e9950e48cec5f48f9e6f9ce3475574ae030da4bb09cda6c9236\",\"kms_data_key_id\":0}',1634807470762,1634807470762,4),(109,1,'eth','USDC 1',NULL,NULL,'0x97032C2c3325Ea26fC499732D667e0B4F4cDACeB',0,0,NULL,'{\"private_key\":\"0xcef2d7a8f26cc76e87c4856f2ee1f954fc94b6727ec8f12a2f63530bb25767e2\",\"kms_data_key_id\":0}',1634807490432,1634807490432,4);
/*!40000 ALTER TABLE `address` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `address_balance`
--

DROP TABLE IF EXISTS `address_balance`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `address_balance` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int NOT NULL,
  `address_id` int NOT NULL,
  `token` varchar(255) NOT NULL,
  `balance` decimal(40,8) unsigned NOT NULL DEFAULT '0.00000000',
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `address_id` (`address_id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB AUTO_INCREMENT=97 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `address_balance`
--

LOCK TABLES `address_balance` WRITE;
/*!40000 ALTER TABLE `address_balance` DISABLE KEYS */;
INSERT INTO `address_balance` VALUES (1,1,60,'btc',0.00000000,1634643497000,1634643497000),(2,1,61,'xrp',0.00000000,1634643504946,1634643504946),(3,1,62,'sol',0.00000000,1634643510529,1634643510529),(4,1,63,'eth',0.00000000,1634643728963,1634643728963),(5,1,63,'usdt',0.00000000,1634643728963,1634643728963),(6,1,63,'usdc',0.00000000,1634643728963,1634643728963),(7,1,64,'eth',0.00000000,1634643732079,1634643732079),(8,1,64,'usdt',0.00000000,1634643732079,1634643732079),(9,1,64,'usdc',0.00000000,1634643732079,1634643732079),(10,1,65,'eth',0.00000000,1634643740964,1634643740964),(11,1,65,'usdt',0.00000000,1634643740964,1634643740964),(12,1,65,'usdc',0.00000000,1634643740964,1634643740964),(13,1,66,'eth',0.00000000,1634669843171,1634669843171),(14,1,66,'usdt',0.00000000,1634669843172,1634669843172),(15,1,66,'usdc',0.00000000,1634669843172,1634669843172),(16,1,67,'eth',0.00000000,1634696597004,1634696597004),(17,1,67,'usdt',0.00000000,1634696597004,1634696597004),(18,1,67,'usdc',0.00000000,1634696597004,1634696597004),(19,1,68,'eth',0.00000000,1634697656421,1634697656421),(20,1,68,'usdt',0.00000000,1634697656421,1634697656421),(21,1,68,'usdc',0.00000000,1634697656421,1634697656421),(22,1,69,'eth',0.00000000,1634697963641,1634697963641),(23,1,69,'usdt',0.00000000,1634697963641,1634697963641),(24,1,69,'usdc',0.00000000,1634697963641,1634697963641),(25,1,70,'btc',0.00000000,1634702132843,1634702132843),(26,1,71,'btc',0.00000000,1634702478333,1634702478333),(27,1,72,'sol',0.00000000,1634702549689,1634702549689),(28,1,73,'eth',0.00000000,1634704900213,1634704900213),(29,1,73,'usdt',0.00000000,1634704900213,1634704900213),(30,1,73,'usdc',0.00000000,1634704900213,1634704900213),(31,1,74,'eth',0.00000000,1634704957334,1634704957334),(32,1,74,'usdt',0.00000000,1634704957334,1634704957334),(33,1,74,'usdc',0.00000000,1634704957334,1634704957334),(34,1,75,'sol',0.00000000,1634705953433,1634705953433),(35,1,76,'btc',0.00000000,1634711073703,1634711073703),(36,1,77,'btc',0.00000000,1634711212427,1634711212427),(37,1,78,'btc',0.00000000,1634712535116,1634712535116),(38,1,79,'eth',0.00000000,1634713337745,1634713337745),(39,1,79,'usdt',0.00000000,1634713337745,1634713337745),(40,1,79,'usdc',0.00000000,1634713337745,1634713337745),(41,1,80,'btc',0.00000000,1634713595920,1634713595920),(42,1,81,'xrp',0.00000000,1634713678647,1634713678647),(43,1,82,'eth',0.00000000,1634713895432,1634713895433),(44,1,82,'usdt',0.00000000,1634713895433,1634713895433),(45,1,82,'usdc',0.00000000,1634713895433,1634713895433),(46,1,83,'xrp',0.00000000,1634713947446,1634713947446),(47,1,84,'btc',0.00000000,1634714397913,1634714397913),(48,1,85,'xrp',0.00000000,1634717363996,1634717363996),(49,1,86,'xrp',0.00000000,1634721402411,1634721402411),(50,1,87,'btc',0.00000000,1634721960191,1634721960191),(51,1,88,'btc',0.00000000,1634722411644,1634722411644),(52,1,89,'sol',0.00000000,1634723226606,1634723226606),(53,1,90,'eth',0.00000000,1634750009156,1634750009156),(54,1,90,'usdt',0.00000000,1634750009156,1634750009156),(55,1,90,'usdc',0.00000000,1634750009156,1634750009156),(56,1,91,'sol',0.00000000,1634783655199,1634783655199),(57,1,92,'eth',0.00000000,1634788084130,1634788084130),(58,1,92,'usdt',0.00000000,1634788084130,1634788084130),(59,1,92,'usdc',0.00000000,1634788084130,1634788084130),(60,1,93,'sol',0.00000000,1634788304723,1634788304723),(61,1,94,'sol',0.00000000,1634788385200,1634788385200),(62,1,95,'sol',0.00000000,1634788494797,1634788494797),(63,1,96,'sol',0.00000000,1634788543837,1634788543837),(64,1,97,'sol',0.00000000,1634798968248,1634798968248),(65,3,98,'eth',0.00000000,1634804437267,1634804437267),(66,3,98,'usdt',0.00000000,1634804437267,1634804437267),(67,3,98,'usdc',0.00000000,1634804437267,1634804437267),(68,1,99,'sol',0.00000000,1634804640320,1634804640320),(69,1,99,'usdc',0.00000000,1634804640320,1634804640320),(70,3,100,'eth',0.00000000,1634805035630,1634805035630),(71,3,100,'usdt',0.00000000,1634805035630,1634805035630),(72,3,100,'usdc',0.00000000,1634805035630,1634805035630),(73,3,101,'eth',0.00000000,1634805804409,1634805804409),(74,3,101,'usdt',0.00000000,1634805804409,1634805804409),(75,3,101,'usdc',0.00000000,1634805804409,1634805804409),(76,3,102,'eth',0.00000000,1634805820389,1634805820389),(77,3,102,'usdt',0.00000000,1634805820389,1634805820389),(78,3,102,'usdc',0.00000000,1634805820389,1634805820389),(79,1,103,'sol',0.00000000,1634806084541,1634806084541),(80,1,103,'usdc',0.00000000,1634806084541,1634806084541),(81,1,104,'eth',0.00000000,1634806126619,1634806126619),(82,1,104,'usdt',0.00000000,1634806126619,1634806126619),(83,1,104,'usdc',0.00000000,1634806126619,1634806126619),(84,1,105,'xrp',0.00000000,1634806202083,1634806202083),(85,3,106,'eth',0.00000000,1634806983730,1634806983730),(86,3,106,'usdt',0.00000000,1634806983730,1634806983730),(87,3,106,'usdc',0.00000000,1634806983730,1634806983730),(88,3,107,'eth',0.00000000,1634807241209,1634807241209),(89,3,107,'usdt',0.00000000,1634807241209,1634807241209),(90,3,107,'usdc',0.00000000,1634807241209,1634807241209),(91,4,108,'eth',0.00000000,1634807470772,1634807470772),(92,4,108,'usdt',0.00000000,1634807470772,1634807470772),(93,4,108,'usdc',0.00000000,1634807470772,1634807470772),(94,4,109,'eth',0.00000000,1634807490444,1634807490444),(95,4,109,'usdt',0.00000000,1634807490444,1634807490444),(96,4,109,'usdc',0.00000000,1634807490444,1634807490444);
/*!40000 ALTER TABLE `address_balance` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `api_key`
--

DROP TABLE IF EXISTS `api_key`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `api_key` (
  `user_id` int NOT NULL,
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `api_key` varchar(250) NOT NULL,
  `api_secret` varchar(250) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `api_key` (`api_key`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `api_key`
--

LOCK TABLES `api_key` WRITE;
/*!40000 ALTER TABLE `api_key` DISABLE KEYS */;
INSERT INTO `api_key` VALUES (3,1634804177166,1634804177166,20,'19da14333fc6e4ebcc56','$argon2i$v=19$m=4096,t=3,p=1$T+ZEUc2F32xQkNEFL4yXRA$GgmCtOYkJa3VEssTvk7e3HALsV/hw0YqIZToeMRtFKY'),(3,1634804288862,1634804288862,21,'ed2c0c082b67f9ff1eba','$argon2i$v=19$m=4096,t=3,p=1$2pJcKxC9tuH5NXZYbG7WOA$5/AUiKB7cN4G2w9aS6J45jVNmNWZit+HqYZNO9lHM3o'),(1,1634804627023,1634804627023,22,'ac42e39291e541e659a5','$argon2i$v=19$m=4096,t=3,p=1$mKg/M7+rT4zL/H/Xy3jQ9g$x7tVurEGWS6xgd+HXg7/MwNNiWYmjJ2riBdpFa9nmf0'),(3,1634806947083,1634806947083,23,'4389ac0b153432d876ee','$argon2i$v=19$m=4096,t=3,p=1$hj3unwSrqQ5hgnCh3s55cw$V6ZdbH0YZVuRxSvNCDPIi29dC02n6BvHfw6XfvP98ow'),(4,1634807430335,1634807430335,24,'bba246689968b8f103f2','$argon2i$v=19$m=4096,t=3,p=1$r/YN+dtgviyJx8b78WkK7w$yFwVgTwzsE4RL3zDGqwJrVQtSjTrDT+6sTTEG51aKSc');
/*!40000 ALTER TABLE `api_key` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `cold_wallet`
--

DROP TABLE IF EXISTS `cold_wallet`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `cold_wallet` (
  `user_id` int NOT NULL,
  `wallet_id` int NOT NULL,
  `address` varchar(190) NOT NULL,
  `currency` varchar(190) NOT NULL,
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  `type` varchar(20) NOT NULL DEFAULT 'normal',
  PRIMARY KEY (`address`),
  KEY `wallet_id` (`wallet_id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `cold_wallet`
--

LOCK TABLES `cold_wallet` WRITE;
/*!40000 ALTER TABLE `cold_wallet` DISABLE KEYS */;
/*!40000 ALTER TABLE `cold_wallet` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `currency`
--

DROP TABLE IF EXISTS `currency`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `currency` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int NOT NULL,
  `wallet_id` int NOT NULL,
  `withdrawal_mode` varchar(90) NOT NULL DEFAULT 'normal',
  `minimum_withdrawal` decimal(40,8) NOT NULL DEFAULT '0.00000000',
  `minimum_collect_amount` decimal(40,8) NOT NULL DEFAULT '0.00000000',
  `lower_threshold` decimal(40,8) NOT NULL DEFAULT '0.00000000',
  `upper_threshold` decimal(40,8) NOT NULL DEFAULT '0.00000000',
  `middle_threshold` decimal(40,8) DEFAULT '0.00000000',
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  `symbol` varchar(100) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `wallet_id` (`wallet_id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `currency`
--

LOCK TABLES `currency` WRITE;
/*!40000 ALTER TABLE `currency` DISABLE KEYS */;
INSERT INTO `currency` VALUES (1,1,1,'normal',0.00000000,0.00000000,900000000000000000.00000000,3000000000000000000.00000000,1000000000000000000.00000000,1618888712703,1623320067659,'eth'),(2,1,2,'normal',0.00000000,0.00000000,900000000000000000.00000000,3000000000000000000.00000000,1000000000000000000.00000000,1618888712703,1623320067659,'btc'),(3,1,3,'normal',0.00000000,0.00000000,900000000000000000.00000000,3000000000000000000.00000000,1000000000000000000.00000000,1618888712703,1623320067659,'xrp'),(4,1,4,'normal',0.00000000,0.00000000,900000000000000000.00000000,3000000000000000000.00000000,1000000000000000000.00000000,1618888712703,1623320067659,'sol');
/*!40000 ALTER TABLE `currency` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `currency_config`
--

DROP TABLE IF EXISTS `currency_config`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `currency_config` (
  `currency` varchar(190) NOT NULL,
  `chain_name` varchar(255) DEFAULT NULL,
  `average_block_time` int NOT NULL,
  `required_confirmations` int NOT NULL,
  `rpc_endpoint` varchar(255) DEFAULT NULL,
  `rest_endpoint` varchar(255) DEFAULT NULL,
  `explorer_endpoint` varchar(255) DEFAULT NULL,
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  `network` varchar(255) NOT NULL,
  `chain_id` varchar(255) DEFAULT NULL,
  `internal_endpoint` varchar(255) NOT NULL,
  PRIMARY KEY (`currency`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `currency_config`
--

LOCK TABLES `currency_config` WRITE;
/*!40000 ALTER TABLE `currency_config` DISABLE KEYS */;
INSERT INTO `currency_config` VALUES ('btc','test',15000,6,'{\"protocol\":\"https\",\"host\":\"https://rinkeby.infura.io/v3/52041a76a2774030beebc0306d44b52d\",\"port\":\"80\",\"user\":\"\",\"pass\":\"\"}','https://rinkeby.infura.io/v3/1ce750f5751c43739f33a2cec55fe9ad','https://rinkeby.etherscan.io',1589874286352,1589874286352,'',NULL,'http://172.31.73.9:47003'),('eth','rinkeby',15000,6,'{\"protocol\":\"https\",\"host\":\"https://rinkeby.infura.io/v3/1ce750f5751c43739f33a2cec55fe9ad\",\"port\":\"80\",\"user\":\"\",\"pass\":\"\"}','https://rinkeby.infura.io/v3/1ce750f5751c43739f33a2cec55fe9ad','https://rinkeby.etherscan.io',1589874286352,1589874286352,'',NULL,'http://172.31.73.9:47002'),('sol','test',15000,6,'{\"protocol\":\"https\",\"host\":\"https://rinkeby.infura.io/v3/52041a76a2774030beebc0306d44b52d\",\"port\":\"80\",\"user\":\"\",\"pass\":\"\"}','https://rinkeby.infura.io/v3/52041a76a2774030beebc0306d44b52d','https://rinkeby.etherscan.io',1589874286352,1589874286352,'',NULL,'http://172.31.73.9:47004'),('xrp','test',15000,6,'{\"protocol\":\"https\",\"host\":\"https://rinkeby.infura.io/v3/52041a76a2774030beebc0306d44b52d\",\"port\":\"80\",\"user\":\"\",\"pass\":\"\"}','https://rinkeby.infura.io/v3/52041a76a2774030beebc0306d44b52d','https://rinkeby.etherscan.io',1589874286352,1589874286352,'',NULL,'http://172.31.73.9:47005');
/*!40000 ALTER TABLE `currency_config` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `deposit`
--

DROP TABLE IF EXISTS `deposit`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `deposit` (
  `wallet_id` int NOT NULL,
  `amount` decimal(32,0) NOT NULL,
  `block_number` int NOT NULL,
  `collect_local_tx_id` int DEFAULT NULL,
  `seed_local_tx_id` int DEFAULT NULL,
  `collect_withdrawal_id` int DEFAULT NULL,
  `collect_type` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  `created_at` bigint NOT NULL,
  `updated_at` bigint NOT NULL,
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `currency` varchar(255) NOT NULL,
  `from_address` text NOT NULL,
  `to_address` varchar(255) NOT NULL,
  `txid` varchar(255) NOT NULL,
  `memo` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  `block_timestamp` int NOT NULL,
  `collect_status` varchar(255) NOT NULL DEFAULT 'uncollected',
  `collected_txid` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  `collected_timestamp` int DEFAULT NULL,
  `seeded_txid` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `deposit`
--

LOCK TABLES `deposit` WRITE;
/*!40000 ALTER TABLE `deposit` DISABLE KEYS */;
INSERT INTO `deposit` VALUES (1,1000000000000000,9503088,NULL,NULL,NULL,NULL,1634806251553,1634806251553,3,'eth','[\"0xAfD3921F6Bae348A5B0fc28f550033A1F49D435e\"]','0xe7491FC2ABE8ac8081B674DFd7EcF3655ae8FF39','0x16a374c40481d7b8c96baff1bd72c3d02ebe0c6cd0f49f213ce531c66009eb36',NULL,1634805920,'uncollected',NULL,NULL,NULL),(1,1000000000000000,9503096,NULL,NULL,NULL,NULL,1634806252638,1634806252638,4,'eth','[\"0xAfD3921F6Bae348A5B0fc28f550033A1F49D435e\"]','0xe7491FC2ABE8ac8081B674DFd7EcF3655ae8FF39','0xe2214080761921598320bd3e7f74951c05c85d71a26938b4850b829faa3e4282',NULL,1634806040,'uncollected',NULL,NULL,NULL),(1,1000000000000000,9503112,NULL,NULL,NULL,NULL,1634806374609,1634806374609,5,'eth','[\"0xAfD3921F6Bae348A5B0fc28f550033A1F49D435e\"]','0xe7491FC2ABE8ac8081B674DFd7EcF3655ae8FF39','0x54e6df41a93e44e3a4004180708a90cbd2e127deeb402308b4f140a2fd8eb3f3',NULL,1634806280,'uncollected',NULL,NULL,NULL);
/*!40000 ALTER TABLE `deposit` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `deposit_log`
--

DROP TABLE IF EXISTS `deposit_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `deposit_log` (
  `deposit_id` int NOT NULL,
  `ref_id` int NOT NULL,
  `created_at` bigint NOT NULL,
  `id` int NOT NULL AUTO_INCREMENT,
  `event` varchar(255) NOT NULL,
  `data` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `deposit_log`
--

LOCK TABLES `deposit_log` WRITE;
/*!40000 ALTER TABLE `deposit_log` DISABLE KEYS */;
INSERT INTO `deposit_log` VALUES (3,3,1634806251555,1,'created',NULL),(4,4,1634806252642,2,'created',NULL),(5,5,1634806374612,3,'created',NULL);
/*!40000 ALTER TABLE `deposit_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `env_config`
--

DROP TABLE IF EXISTS `env_config`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `env_config` (
  `key` varchar(255) NOT NULL,
  `value` varchar(255) DEFAULT NULL,
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  PRIMARY KEY (`key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `env_config`
--

LOCK TABLES `env_config` WRITE;
/*!40000 ALTER TABLE `env_config` DISABLE KEYS */;
INSERT INTO `env_config` VALUES ('MAIL_FROM_ADDRESS','noreply@sotatek.com',NULL,NULL),('MAIL_FROM_NAME','Wallet Support',NULL,NULL),('MAIL_PASSWORD','piclqphxxhluqsil',NULL,NULL),('MAIL_RECIPIENT_COLD_WALLET_1','testekoios@gmail.com',NULL,NULL),('MAIL_RECIPIENT_COLD_WALLET_2','hang.nguyen@ekoios.vn',NULL,NULL),('MAIL_RECIPIENT_COLD_WALLET_NUM','2',NULL,NULL),('MAIL_RECIPIENT_ERROR_ALERT','testekoios@gmail.com',NULL,NULL),('MAIL_USERNAME','sotatek.test@gmail.com',NULL,NULL),('MAILER_ACCOUNT','sotatek.test',NULL,NULL),('MAILER_HOST','smtp.gmail.com',NULL,NULL),('MAILER_PASSWORD','piclqphxxhluqsil',NULL,NULL),('MAILER_PORT','465',NULL,NULL),('MAILER_RECEIVER','testekoios@gmail.com',NULL,NULL),('NETWORK','testnet',NULL,NULL),('REDIS_ENABLED','true',NULL,NULL),('REDIS_HOST','redis',NULL,NULL),('REDIS_PORT','6379',NULL,NULL),('REDIS_URL','redis://127.0.0.1:6379',NULL,NULL),('WEBHOOK_PROGRESS_RETRY_COUNT','10',NULL,NULL),('WEBHOOK_RECORDS_TO_PROCESS','100',NULL,NULL),('WEBHOOK_REQUEST_PASSWORD','123',NULL,NULL),('WEBHOOK_REQUEST_USER','abc',NULL,NULL);
/*!40000 ALTER TABLE `env_config` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `erc20_token`
--

DROP TABLE IF EXISTS `erc20_token`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `erc20_token` (
  `contract_address` varchar(255) NOT NULL,
  `symbol` varchar(20) NOT NULL,
  `name` varchar(100) NOT NULL,
  `decimal` tinyint unsigned NOT NULL,
  `total_supply` decimal(32,0) NOT NULL DEFAULT '0',
  `network` varchar(20) NOT NULL,
  PRIMARY KEY (`contract_address`),
  UNIQUE KEY `IDX_5ea1a3f07270d24ff5c04b800f` (`symbol`),
  UNIQUE KEY `IDX_664a95f77e5f26894243be4892` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `erc20_token`
--

LOCK TABLES `erc20_token` WRITE;
/*!40000 ALTER TABLE `erc20_token` DISABLE KEYS */;
/*!40000 ALTER TABLE `erc20_token` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `hot_wallet`
--

DROP TABLE IF EXISTS `hot_wallet`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `hot_wallet` (
  `user_id` int NOT NULL,
  `wallet_id` int NOT NULL,
  `currency` varchar(200) NOT NULL,
  `type` varchar(20) NOT NULL DEFAULT 'normal',
  `secret` text NOT NULL,
  `balance` decimal(40,8) unsigned NOT NULL DEFAULT '0.00000000',
  `is_external` tinyint(1) NOT NULL DEFAULT '0',
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  `address` varchar(255) NOT NULL,
  PRIMARY KEY (`address`),
  KEY `wallet_id` (`wallet_id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `hot_wallet`
--

LOCK TABLES `hot_wallet` WRITE;
/*!40000 ALTER TABLE `hot_wallet` DISABLE KEYS */;
/*!40000 ALTER TABLE `hot_wallet` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kms_cmk`
--

DROP TABLE IF EXISTS `kms_cmk`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `kms_cmk` (
  `id` varchar(191) NOT NULL,
  `region` varchar(255) NOT NULL,
  `alias` varchar(255) DEFAULT '',
  `arn` varchar(255) NOT NULL,
  `is_enabled` tinyint(1) NOT NULL DEFAULT '0',
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `region` (`region`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `kms_cmk`
--

LOCK TABLES `kms_cmk` WRITE;
/*!40000 ALTER TABLE `kms_cmk` DISABLE KEYS */;
/*!40000 ALTER TABLE `kms_cmk` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kms_data_key`
--

DROP TABLE IF EXISTS `kms_data_key`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `kms_data_key` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `cmk_id` varchar(255) NOT NULL,
  `encrypted_data_key` varchar(500) NOT NULL,
  `is_enabled` tinyint(1) NOT NULL DEFAULT '1',
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `kms_data_key`
--

LOCK TABLES `kms_data_key` WRITE;
/*!40000 ALTER TABLE `kms_data_key` DISABLE KEYS */;
/*!40000 ALTER TABLE `kms_data_key` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `latest_block`
--

DROP TABLE IF EXISTS `latest_block`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `latest_block` (
  `block_number` int NOT NULL,
  `created_at` bigint NOT NULL,
  `updated_at` bigint NOT NULL,
  `currency` varchar(255) NOT NULL,
  `type` varchar(255) NOT NULL,
  PRIMARY KEY (`currency`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `latest_block`
--

LOCK TABLES `latest_block` WRITE;
/*!40000 ALTER TABLE `latest_block` DISABLE KEYS */;
/*!40000 ALTER TABLE `latest_block` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `local_tx`
--

DROP TABLE IF EXISTS `local_tx`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `local_tx` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int NOT NULL,
  `wallet_id` int NOT NULL,
  `from_address` varchar(255) NOT NULL,
  `to_address` varchar(255) NOT NULL,
  `currency` varchar(200) NOT NULL,
  `currency_symbol` varchar(100) NOT NULL,
  `amount` decimal(40,8) DEFAULT NULL,
  `type` varchar(40) NOT NULL,
  `ref_currency` varchar(200) NOT NULL,
  `ref_currency_symbol` varchar(100) NOT NULL,
  `ref_table` varchar(100) DEFAULT NULL,
  `ref_id` int NOT NULL,
  `memo` varchar(50) DEFAULT NULL,
  `status` varchar(20) NOT NULL,
  `unsigned_txid` varchar(100) NOT NULL,
  `block_number` bigint DEFAULT NULL,
  `block_hash` varchar(100) DEFAULT NULL,
  `block_timestamp` bigint DEFAULT NULL,
  `fee_amount` decimal(40,8) DEFAULT NULL,
  `fee_currency` varchar(200) DEFAULT NULL,
  `unsigned_raw` text,
  `signed_raw` text,
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  `txid` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `wallet_id` (`wallet_id`),
  KEY `user_id` (`user_id`),
  KEY `updated_at` (`updated_at`),
  KEY `unsigned_txid` (`unsigned_txid`),
  KEY `txid` (`txid`),
  KEY `to_address` (`to_address`),
  KEY `ref_id` (`ref_id`),
  KEY `from_address` (`from_address`),
  KEY `created_at` (`created_at`),
  KEY `block_timestamp` (`block_timestamp`),
  KEY `block_number` (`block_number`),
  KEY `block_hash` (`block_hash`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `local_tx`
--

LOCK TABLES `local_tx` WRITE;
/*!40000 ALTER TABLE `local_tx` DISABLE KEYS */;
/*!40000 ALTER TABLE `local_tx` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mail_job`
--

DROP TABLE IF EXISTS `mail_job`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `mail_job` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `sender_name` varchar(100) DEFAULT NULL,
  `sender_address` varchar(100) NOT NULL,
  `recipient_address` varchar(100) NOT NULL,
  `template_name` varchar(50) NOT NULL,
  `content` text,
  `is_sent` tinyint(1) DEFAULT '0',
  `retry_count` tinyint DEFAULT '0',
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  `title` varchar(254) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mail_job`
--

LOCK TABLES `mail_job` WRITE;
/*!40000 ALTER TABLE `mail_job` DISABLE KEYS */;
/*!40000 ALTER TABLE `mail_job` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mail_log`
--

DROP TABLE IF EXISTS `mail_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `mail_log` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `job_id` int NOT NULL,
  `msg` text,
  `created_at` bigint DEFAULT NULL,
  `status` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `job_id` (`job_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mail_log`
--

LOCK TABLES `mail_log` WRITE;
/*!40000 ALTER TABLE `mail_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `mail_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `migrations`
--

DROP TABLE IF EXISTS `migrations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `migrations` (
  `id` int NOT NULL AUTO_INCREMENT,
  `timestamp` bigint NOT NULL,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `migrations`
--

LOCK TABLES `migrations` WRITE;
/*!40000 ALTER TABLE `migrations` DISABLE KEYS */;
INSERT INTO `migrations` VALUES (2,1614566017687,'ColdWalletSchema1614566017687'),(3,1614566707346,'CurrencySchema1614566707346'),(4,1614567807123,'CurrencyConfigSchema1614567807123'),(5,1614568315485,'DepositSchema1614568315485'),(6,1614568862846,'DepositLogSchema1614568862846'),(7,1614569202014,'EnvConfigSchema1614569202014'),(8,1614569343965,'Erc20TokenSchema1614569343965'),(9,1614569638931,'HotWalletSchema1614569638931'),(10,1614570053358,'KmsCmkSchema1614570053358'),(11,1614570436277,'KmsDataKeySchema1614570436277'),(12,1614570611471,'LatestBlockSchema1614570611471'),(13,1614570728119,'LocalTxSchema1614570728119'),(14,1614571830084,'MailJobSchema1614571830084'),(15,1614572267796,'MailLogSchema1614572267796'),(16,1614572970214,'UserSchema1614572970214'),(17,1614573622856,'UserCurrencySchema1614573622856'),(18,1614573933230,'WalletSchema1614573933230'),(19,1614574162495,'WalletBalanceSchema1614574162495'),(20,1614574510601,'WalletLogSchema1614574510601'),(21,1614580544156,'WebhookSchema1614580544156'),(22,1614580735640,'WebhookLogSchema1614580735640'),(23,1614581023976,'WebhookProgressSchema1614581023976'),(24,1614581245790,'WithdrawalSchema1614581245790'),(25,1614581730301,'WithdrawalLogSchema1614581730301'),(26,1614581897632,'WithdrawalTxSchema1614581897632'),(27,1614594120943,'RallyWalletSchema1614594120943'),(29,1614564718572,'AddressSchema1614564718572'),(30,1614594120944,'ApiKeySchema1614594120944');
/*!40000 ALTER TABLE `migrations` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `rally_wallet`
--

DROP TABLE IF EXISTS `rally_wallet`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `rally_wallet` (
  `user_id` int NOT NULL,
  `wallet_id` int NOT NULL,
  `created_at` bigint NOT NULL,
  `updated_at` bigint NOT NULL,
  `address` varchar(255) NOT NULL,
  `currency` varchar(255) NOT NULL,
  `type` varchar(255) NOT NULL,
  `withdrawal_mode` varchar(255) NOT NULL,
  PRIMARY KEY (`address`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `rally_wallet`
--

LOCK TABLES `rally_wallet` WRITE;
/*!40000 ALTER TABLE `rally_wallet` DISABLE KEYS */;
/*!40000 ALTER TABLE `rally_wallet` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `user` (
  `username` varchar(80) NOT NULL,
  `email` varchar(191) NOT NULL,
  `password` varchar(255) NOT NULL,
  `avatar_url` varchar(255) DEFAULT NULL,
  `full_name` varchar(100) DEFAULT NULL,
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  `id` int NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`),
  UNIQUE KEY `IDX_78a916df40e02a9deb1c4b75ed` (`username`),
  UNIQUE KEY `IDX_e12875dfb3b1d92d7d7c5377e2` (`email`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user`
--

LOCK TABLES `user` WRITE;
/*!40000 ALTER TABLE `user` DISABLE KEYS */;
INSERT INTO `user` VALUES ('gakki','example@gmail.com','$argon2i$v=19$m=4096,t=3,p=1$56kARAgfgwL0IjnemPVYsA$05hUBsm8ocD8Ar8KlrudllKeZsCjVbuiWgBv+dB94jU',NULL,NULL,1618887054712,1618887054712,1),('Test01','no1@mailneisa.com','$argon2i$v=19$m=4096,t=3,p=1$ZIfHQtOCsJGpfVhsbL21dw$cf6UkNDQB2al9RzH2ZsBwdPe0Ifnwp6I4F+t1wBKTow',NULL,NULL,1634037214290,1634037214290,2),('trientt','trien.tran@ekoios.vn','$argon2i$v=19$m=4096,t=3,p=1$3YicdoVVnt8ux3WaVn2KgQ$QCCnGEA9AcZvFd5le8+iDdOeobrhcx8VJYl+t26C9Bg',NULL,NULL,1634523684175,1634523684175,3),('hangnt','hang.nguyen@ekoios.vn','$argon2i$v=19$m=4096,t=3,p=1$CqU6t0WhUOaA0s/ICiz2+Q$UPVtX4WyluX0ElqdmSR9RpFBTYL+FoblOphP2gsTBX0',NULL,NULL,1634807380796,1634807380796,4);
/*!40000 ALTER TABLE `user` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_currency`
--

DROP TABLE IF EXISTS `user_currency`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `user_currency` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int NOT NULL,
  `system_symbol` varchar(190) NOT NULL,
  `custom_symbol` varchar(20) NOT NULL,
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `user_id_system_symbol_unique` (`user_id`,`system_symbol`),
  UNIQUE KEY `user_id_custom_symbol_unique` (`user_id`,`custom_symbol`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_currency`
--

LOCK TABLES `user_currency` WRITE;
/*!40000 ALTER TABLE `user_currency` DISABLE KEYS */;
/*!40000 ALTER TABLE `user_currency` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `wallet`
--

DROP TABLE IF EXISTS `wallet`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `wallet` (
  `user_id` int NOT NULL,
  `label` varchar(255) NOT NULL,
  `withdrawal_mode` varchar(90) NOT NULL DEFAULT 'normal',
  `secret` text,
  `is_hd` tinyint(1) DEFAULT '0',
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `currency` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `wallet`
--

LOCK TABLES `wallet` WRITE;
/*!40000 ALTER TABLE `wallet` DISABLE KEYS */;
INSERT INTO `wallet` VALUES (1,'eth-wallet','normal',NULL,0,1618888712699,1618888712699,1,'eth'),(1,'btc-wallet','normal',NULL,0,1618888712699,1618888712699,2,'btc'),(1,'xrp-wallet','normal',NULL,0,1618888712699,1618888712699,3,'xrp'),(1,'sol-wallet','normal',NULL,0,1618888712699,1618888712699,4,'sol');
/*!40000 ALTER TABLE `wallet` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `wallet_balance`
--

DROP TABLE IF EXISTS `wallet_balance`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `wallet_balance` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `wallet_id` int NOT NULL,
  `currency` varchar(190) NOT NULL,
  `balance` decimal(40,8) NOT NULL DEFAULT '0.00000000',
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `wallet_id_currency_unique` (`wallet_id`,`currency`),
  KEY `wallet_id` (`wallet_id`)
) ENGINE=InnoDB AUTO_INCREMENT=5115 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `wallet_balance`
--

LOCK TABLES `wallet_balance` WRITE;
/*!40000 ALTER TABLE `wallet_balance` DISABLE KEYS */;
INSERT INTO `wallet_balance` VALUES (3,1,'eth',0.00000000,1633009085,1633009085),(3440,2,'btc',0.00000000,1634284250,1634284250),(4446,3,'xrp',0.00000000,1634633544,1634633544),(4465,4,'sol',0.00000000,1634633544,1634633544);
/*!40000 ALTER TABLE `wallet_balance` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `wallet_log`
--

DROP TABLE IF EXISTS `wallet_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `wallet_log` (
  `wallet_id` int NOT NULL,
  `currency` varchar(200) NOT NULL,
  `ref_currency` varchar(200) NOT NULL,
  `event` varchar(30) NOT NULL,
  `balance_change` decimal(40,8) DEFAULT NULL,
  `ref_id` bigint NOT NULL,
  `data` text,
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`),
  KEY `ref_id` (`ref_id`),
  KEY `wallet_id` (`wallet_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `wallet_log`
--

LOCK TABLES `wallet_log` WRITE;
/*!40000 ALTER TABLE `wallet_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `wallet_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `webhook`
--

DROP TABLE IF EXISTS `webhook`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `webhook` (
  `user_id` int NOT NULL,
  `type` varchar(20) NOT NULL DEFAULT 'common',
  `url` varchar(255) NOT NULL,
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `webhook`
--

LOCK TABLES `webhook` WRITE;
/*!40000 ALTER TABLE `webhook` DISABLE KEYS */;
/*!40000 ALTER TABLE `webhook` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `webhook_log`
--

DROP TABLE IF EXISTS `webhook_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `webhook_log` (
  `webhook_progress_id` int NOT NULL,
  `url` varchar(255) NOT NULL,
  `params` text,
  `status` int NOT NULL,
  `msg` text,
  `created_at` bigint DEFAULT NULL,
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`),
  KEY `webhook_progress_id` (`webhook_progress_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `webhook_log`
--

LOCK TABLES `webhook_log` WRITE;
/*!40000 ALTER TABLE `webhook_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `webhook_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `webhook_progress`
--

DROP TABLE IF EXISTS `webhook_progress`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `webhook_progress` (
  `webhook_id` int NOT NULL,
  `ref_id` int NOT NULL,
  `is_processed` tinyint(1) NOT NULL DEFAULT '0',
  `retry_count` int NOT NULL,
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `type` varchar(255) NOT NULL,
  `event` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `ref_id` (`ref_id`),
  KEY `webhook_id` (`webhook_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `webhook_progress`
--

LOCK TABLES `webhook_progress` WRITE;
/*!40000 ALTER TABLE `webhook_progress` DISABLE KEYS */;
/*!40000 ALTER TABLE `webhook_progress` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `withdrawal`
--

DROP TABLE IF EXISTS `withdrawal`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `withdrawal` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int NOT NULL,
  `wallet_id` int NOT NULL,
  `withdrawal_tx_id` int NOT NULL DEFAULT '0',
  `txid` varchar(100) NOT NULL DEFAULT '',
  `amount` decimal(40,8) NOT NULL,
  `status` varchar(20) NOT NULL,
  `memo` varchar(50) DEFAULT NULL,
  `type` varchar(50) NOT NULL,
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  `currency` varchar(255) NOT NULL,
  `from_address` varchar(100) NOT NULL DEFAULT '',
  `to_address` varchar(100) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `to_address` (`to_address`),
  KEY `from_address` (`from_address`),
  KEY `withdrawal_tx_id` (`withdrawal_tx_id`),
  KEY `wallet_id` (`wallet_id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `withdrawal`
--

LOCK TABLES `withdrawal` WRITE;
/*!40000 ALTER TABLE `withdrawal` DISABLE KEYS */;
/*!40000 ALTER TABLE `withdrawal` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `withdrawal_log`
--

DROP TABLE IF EXISTS `withdrawal_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `withdrawal_log` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `withdrawal_id` int NOT NULL,
  `ref_id` int NOT NULL,
  `data` text,
  `created_at` bigint DEFAULT NULL,
  `event` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `created_at` (`created_at`),
  KEY `ref_id` (`ref_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `withdrawal_log`
--

LOCK TABLES `withdrawal_log` WRITE;
/*!40000 ALTER TABLE `withdrawal_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `withdrawal_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `withdrawal_tx`
--

DROP TABLE IF EXISTS `withdrawal_tx`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `withdrawal_tx` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int NOT NULL,
  `wallet_id` int NOT NULL,
  `hot_wallet_address` varchar(255) NOT NULL,
  `txid` varchar(100) DEFAULT NULL,
  `currency` varchar(200) NOT NULL,
  `status` varchar(20) NOT NULL,
  `unsigned_txid` varchar(100) NOT NULL,
  `block_number` int DEFAULT NULL,
  `block_hash` varchar(100) DEFAULT NULL,
  `block_timestamp` bigint DEFAULT NULL,
  `fee_amount` decimal(40,8) DEFAULT NULL,
  `fee_currency` varchar(200) DEFAULT NULL,
  `unsigned_raw` text,
  `signed_raw` text,
  `created_at` bigint DEFAULT NULL,
  `updated_at` bigint DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `IDX_5a9768d58b6cc05d94334c7cd0` (`unsigned_txid`),
  UNIQUE KEY `unsigned_txid_unique` (`unsigned_txid`),
  UNIQUE KEY `IDX_e89e79f5440478d01e80c5278b` (`txid`),
  UNIQUE KEY `txid_unique` (`txid`),
  KEY `updated_at` (`updated_at`),
  KEY `created_at` (`created_at`),
  KEY `hot_wallet_address` (`hot_wallet_address`),
  KEY `wallet_id` (`wallet_id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `withdrawal_tx`
--

LOCK TABLES `withdrawal_tx` WRITE;
/*!40000 ALTER TABLE `withdrawal_tx` DISABLE KEYS */;
/*!40000 ALTER TABLE `withdrawal_tx` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'x_wallet'
--
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2021-10-21 16:14:50
