const short_item_record_type item_list[370] = {
{item_variety::Pants,0, 0,0,0,0,0, 23,0,0,0, 2, 1,0, 6,99, "Pants", "Pants" ,0,0, 0,0,0,0},
{item_variety::Pants,0, 0,0,0,0,0, 24,0,0,0, 2, 1,0, 7,99, "Pants", "Pants" ,0,0, 0,0,0,0}, // 5
{item_variety::Armor,0, 0,0,0,0,0, 73,0,0,0, 2, 1,0, 5,98, "Shirt", "Shirt" ,0,0, 0,0,0,0},
{item_variety::Armor,0, 0,0,0,0,0, 74,0,0,0, 2, 1,0, 6,98, "Shirt", "Shirt" ,0,0, 0,0,0,0}, 
{item_variety::Armor,1, 0,0,0,0,0, 22,0,0,0, 3, 1,0, 14,100, "Robes", "Robes" ,0,0, 0,0,0,0},
{item_variety::ThrownMissile,3, 0,0,0,0,0, 71,0,15,0, 0, 1,0, 3,29, "Rock", "Rock" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,4, 0,0,0,0,2, 70,0,0,0, 0, 1,0, 8,101, "Stick", "Stick" ,0,0, 0,0,0,0}, 
{item_variety::GemStoneEtc,0, 0,0,0,0,0, 76,0,0,0, 0, 1,0, 4,102, "Mug", "Mug" ,0,0, 0,0,0,0},
{item_variety::GemStoneEtc,0, 0,0,0,0,0, 77,0,0,0, 0, 1,0, 5,103, "Plate", "Plate" ,0,0, 0,0,0,0},
{item_variety::GemStoneEtc,0, 0,0,0,0,0, 81,0,0,0, 0, 1,0, 3,104, "Bottle", "Bottle" ,0,0, 0,0,0,0},
{item_variety::Tool,1, 0,0,0,1,0, 82,12,38,0, 1, 1,0, 2,105, "Candle", "Candle" ,0,0, 0,2,0,90},
{item_variety::Tool,8, 0,0,0,1,0, 83,12,0,0, 8, 1,0, 14,106, "Lamp", "Lamp" ,0,0, 0,7,0,90},
{item_variety::GemStoneEtc,0, 0,0,0,0,0, 88,0,0,0, 0, 1,0, 7,107, "Garbage", "Garbage" ,0,0, 0,0,0,0},
{item_variety::GemStoneEtc,0, 0,0,0,0,0, 68,0,0,0, 0, 1,0, 18,108, "Bones", "Bones" ,0,0, 0,0,0,0},
{item_variety::GemStoneEtc,0, 0,0,0,0,0, 89,0,0,0, 0, 1,0, 6,108, "Skull", "Skull" ,0,0, 0,0,0,0},
{item_variety::Tool,2, 0,0,0,6,0, 37,12,13,0, 2, 1,0, 3,26, "Torches", "Torches" ,0,0, 0,5,0,90}, // 25
{item_variety::Tool,2, 0,0,0,3,0, 37,12,13,0, 2, 1,0, 3,26, "Torches", "Torches" ,0,0, 0,5,0,90}, 
{item_variety::Armor,2, 0,0,0,0,0, 18,0,0,0, 8, 1,0, 10,126, "Vahnatai Robes", "Robes" ,0,0, 0,0,0,0}, // 30
{item_variety::OneHandedWeapon,4,0,0,0,0,1,25,0,0,0,2,1,0,4,78,"Stone Knife","Knife" ,0,0, 0,0,1,0}, // 35
{item_variety::OneHandedWeapon,6,0,0,0,0,1,0,0,0,0,5,1,0,15,79,"Stone Short Sword","Short Sword" ,0,0, 0,0,1,0},
{item_variety::OneHandedWeapon,5,0,0,0,0,2,62,0,0,0,2,1,0,12,80,"Stone Club","Club" ,0,0, 0,0,1,0},
{item_variety::OneHandedWeapon,7,0,0,0,0,2,2,0,0,0,5,1,0,18,81,"Stone Mace","Mace" ,0,0, 0,0,1,0},
{item_variety::OneHandedWeapon,7,0,0,0,0,1,3,0,0,0,6,1,0,20,82,"Stone Axe","Axe" ,0,0, 0,0,1,0},
{item_variety::TwoHandedWeapon,9,0,0,0,0,3,4,0,0,0,10,1,0,30,83,"Stone Spear","Spear" ,0,0, 0,0,1,0}, // 40
{item_variety::OneHandedWeapon,4,0,1,0,0,1,25,0,0,0,16,0,0,7,110,"Bronze Knife","Knife" ,0,0, 0,0,1,0},
{item_variety::OneHandedWeapon,6,0,1,0,0,1,0,0,0,0,20,0,0,15,111,"Bronze Short Sword","Short Sword" ,0,0, 0,0,1,0},
{item_variety::OneHandedWeapon,5,0,1,0,0,2,62,0,0,0,12,0,0,12,80,"Bronze Club","Club" ,0,0, 0,0,1,0},
{item_variety::OneHandedWeapon,7,0,1,0,0,2,2,0,0,0,22,0,0,18,81,"Bronze Mace","Mace" ,0,0, 0,0,1,0},
{item_variety::OneHandedWeapon,7,0,1,0,0,1,3,0,0,0,30,0,0,20,82,"Bronze Axe","Axe" ,0,0, 0,0,2,0}, // 45
{item_variety::TwoHandedWeapon,9,0,1,0,0,3,4,0,0,0,40,0,0,30,83,"Bronze Spear","Spear" ,0,0, 0,0,2,0},
{item_variety::OneHandedWeapon,8,0,1,0,0,2,2,0,0,0,30,0,0,25,84,"Bronze Hammer","Hammer" ,0,0, 0,0,2,0},
{item_variety::OneHandedWeapon,7,0,1,0,0,1,0,0,0,0,30,0,0,15,85,"Bronze Rapier","Rapier" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,9,0,1,0,0,1,0,0,0,0,50,0,0,25,86,"Bronze Broadsword","Broadsword" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,9,0,1,0,0,2,9,0,0,0,45,0,0,30,87,"Bronze Flail","Flail" ,0,0, 0,0,0,0}, // 50
{item_variety::TwoHandedWeapon,12,0,1,0,0,3,8,0,0,0,200,0,0,35,88,"Bronze Bardiche","Bardiche" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,18,0,1,0,0,3,5,0,0,0,450,0,0,50,89,"Bronze Halberd","Halberd" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,14,0,1,0,0,1,1,0,0,0,400,0,0,45,90,"Bronze Greatsword","Greatsword" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,13,0,1,0,0,2,2,0,0,0,375,0,0,45,91,"Bronze Great Mace","Great Mace" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,4,0,2,0,0,1,25,0,0,0,35,0,0,8,110,"Iron Knife","Knife" ,0,0, 0,0,0,0}, // 55
{item_variety::OneHandedWeapon,6,0,2,0,0,1,0,0,0,0,50,0,0,15,111,"Iron Short Sword","Short Sword" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,7,0,2,0,0,2,2,0,0,0,70,0,0,20,81,"Iron Mace","Mace" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,7,0,2,0,0,1,3,0,0,0,80,0,0,20,82,"Iron Axe","Axe" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,9,0,2,0,0,3,4,0,0,0,140,0,0,30,83,"Iron Spear","Spear" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,8,0,2,0,0,2,2,0,0,0,80,0,0,25,84,"Iron Hammer","Hammer" ,0,0, 0,0,0,0}, //60
{item_variety::OneHandedWeapon,7,0,2,0,0,1,0,0,0,0,80,0,0,18,85,"Iron Rapier","Rapier" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,9,0,2,0,0,1,0,0,0,0,200,0,0,25,86,"Iron Broadsword","Broadsword" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,9,0,2,0,0,2,9,0,0,0,200,0,0,30,87,"Iron Flail","Flail" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,12,0,2,0,0,3,8,0,0,0,600,0,0,40,88,"Iron Bardiche","Bardiche" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,18,0,2,0,0,3,5,0,0,0,1000,0,0,50,89,"Iron Halberd","Halberd" ,0,0, 0,0,0,0}, //65
{item_variety::TwoHandedWeapon,14,0,2,0,0,1,1,0,0,0,900,0,0,50,90,"Iron Greatsword","Greatsword" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,13,0,2,0,0,2,2,0,0,0,850,0,0,45,91,"Iron Great Mace","Great Mace" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,4,0,3,0,0,1,25,0,0,0,70,0,0,8,110,"Steel Knife","Knife" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,6,0,3,0,0,1,0,0,0,0,120,0,0,15,111,"Steel Short Sword","Short Sword" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,7,0,3,0,0,2,2,0,0,0,170,0,0,20,81,"Steel Mace","Mace" ,0,0, 0,0,0,0}, // 70
{item_variety::OneHandedWeapon,7,0,3,0,0,1,3,0,0,0,200,0,0,20,82,"Steel Axe","Axe" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,9,0,3,0,0,3,4,0,0,0,340,0,0,30,83,"Steel Spear","Spear" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,8,0,3,0,0,2,2,0,0,0,200,0,0,25,84,"Steel Hammer","Hammer" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,7,0,3,0,0,1,0,0,0,0,200,0,0,18,85,"Steel Rapier","Rapier" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,9,0,3,0,0,1,0,0,0,0,500,0,0,25,86,"Steel Broadsword","Broadsword" ,0,0, 0,0,0,0}, //75
{item_variety::OneHandedWeapon,9,0,3,0,0,2,9,0,0,0,500,0,0,30,87,"Steel Flail","Flail" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,12,0,3,0,0,3,8,0,0,0,1500,0,0,40,88,"Steel Bardiche","Bardiche" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,18,0,3,0,0,3,5,0,0,0,2000,0,0,50,89,"Steel Halberd","Halberd" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,14,0,3,0,0,1,1,0,0,0,1800,0,0,50,90,"Steel Greatsword","Greatsword" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,13,0,3,0,0,2,2,0,0,0,1700,0,0,50,91,"Steel Great Mace","Great Mace" ,0,0, 0,0,0,0}, // 80
{item_variety::OneHandedWeapon,4,0,5,0,0,1,25,0,0,0,150,0,1,8,110,"Magic Knife","Knife" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,6,0,5,0,0,1,0,0,0,0,250,0,1,15,111,"Magic Short Sword","Short Sword" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,7,0,5,0,0,2,2,0,0,0,350,0,1,20,81,"Magic Mace","Mace" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,7,0,5,0,0,1,3,0,0,0,400,0,1,20,82,"Magic Axe","Axe" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,9,0,5,0,0,3,4,0,0,0,700,0,1,30,83,"Magic Spear","Spear" ,0,0, 0,0,0,0}, // 85
{item_variety::OneHandedWeapon,8,0,5,0,0,2,2,0,0,0,400,0,1,25,84,"Magic Hammer","Hammer" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,7,0,5,0,0,1,0,0,0,0,400,0,1,18,85,"Magic Rapier","Rapier" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,9,0,5,0,0,1,0,0,0,0,1000,0,1,25,86,"Magic Broadsword","Broadsword" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,9,0,5,0,0,2,9,0,0,0,1000,0,1,30,87,"Magic Flail","Flail" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,12,0,5,0,0,3,8,0,0,0,3000,0,1,40,88,"Magic Bardiche","Bardiche" ,0,0, 0,0,0,0},  // 90
{item_variety::TwoHandedWeapon,18,0,5,0,0,3,5,0,0,0,4000,0,1,50,89,"Magic Halberd","Halberd" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,14,0,5,0,0,1,1,0,0,0,3600,0,1,45,90,"Magic Greatsword","Greatsword" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,13,0,5,0,0,2,2,0,0,0,3400,0,1,45,91,"Magic Great Mace","Great Mace" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,12,0,2,0,0,3,6,0,0,0,400,0,0,40,95,"Iron Slith Spear","Slith Spear" ,0,0, 0,0,0,0},
{item_variety::TwoHandedWeapon,12,0,3,0,0,3,6,0,0,0,750,0,0,40,95,"Steel Slith Spear","Slith Spear" ,0,0, 0,0,0,0}, // 95
{item_variety::OneHandedWeapon,10,0,1,0,0,1,7,0,0,0,50,0,0,20,94,"Bronze Wave Blade","Wave Blade" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,10,0,2,0,0,1,7,0,0,0,200,0,0,20,94,"Iron Wave Blade","Wave Blade" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,10,0,3,0,0,1,7,0,0,0,500,0,0,20,94,"Steel Wave Blade","Wave Blade" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,10,0,5,0,0,1,7,0,0,0,1000,0,1,20,94,"Magic Wave Blade","Wave Blade" ,0,0, 0,0,0,0},
{item_variety::ThrownMissile,9,0,1,0,8,0,29,0,9,0,10,0,0,1,6,"Iron Razordisks","Razordisks" ,0,0, 0,0,0,0}, // 100
{item_variety::ThrownMissile,9,0,2,0,8,0,29,0,10,0,22,0,0,1,6,"Steel Razordisks","Razordisks" ,0,0, 0,0,0,0},
{item_variety::ThrownMissile,4,0,0,0,10,0,28,0,1,0,1,0,0,1,4,"Darts","Darts" ,0,0, 0,0,0,0},
{item_variety::ThrownMissile,4,0,1,0, 8,0,28,0,2, 0,3,0,0,1,4,"Iron Darts","Darts" ,0,0, 0,0,0,0},
{item_variety::ThrownMissile,4,0,5,0, 6,0,28,0,3, 0,15,0,1,1,5,"Magic Darts","Darts" ,0,0, 0,0,0,0},
{item_variety::ThrownMissile,7,0,0,0, 10,0,25,0,4, 0,2,0,0,2,96,"Throwing Knives","Small Knives" ,0,0, 0,0,0,0}, //105
{item_variety::ThrownMissile,7,0,2,0,10,0,25,0,5,0,7,0,0,2,96,"Iron Throwing Knives","Small Knives" ,0,0, 0,0,0,0},
{item_variety::Arrows,9,0,0,0,12,0,27,0,6,0,1,0,0,1,2,"Arrows","Arrows" ,0,0, 0,0,0,0},
{item_variety::Arrows,9,0,2,0,10,0,27,0,7,0,20,0,0,1,2,"Iron Arrows","Arrows" ,0,0, 0,0,0,0},
{item_variety::Arrows,9,0,5,0,8,0,27,0,8,0,40,0,1,1,3,"Magic Arrows","Arrows" ,0,0, 0,0,0,0},
{item_variety::ThrownMissile,8,0,0,0,8,0,11,0,11,0,2,0,0,3,7,"Javelins","Javelins" ,0,0, 0,0,0,0}, // 110
{item_variety::ThrownMissile,8,0,1,0,8,0,11,0,12,0,12,0,0,3,7,"Iron Javelins","Javelins" ,0,0, 0,0,0,0},
{item_variety::Bow,0,0,0,0,0,0,10,0,0,0,30,0,0,20,1,"Cavewood Bow","Bow" ,0,0, 0,0,0,0},
{item_variety::Bow,0,0,1,0,0,0,10,0,0,0,120,0,0,20,1,"Lemonwood Bow","Bow" ,0,0, 0,0,0,0},
{item_variety::Bow,0,0,2,0,0,0,10,0,0,0,240,0,0,20,1,"Yew Bow","Bow" ,0,0, 0,0,0,0},
{item_variety::Bow,0,0,4,0,0,0,10,0,0,0,1000,0,1,10,1,"Magic Bow","Bow" ,0,0, 0,0,0,0}, // 115
{item_variety::Crossbow,0,0,0,0,0,0,21,0,0,0,80,0,0,35,93,"Crossbow","Crossbow" ,0,0, 0,0,0,0},
{item_variety::Crossbow,0,0,1,0,0,0,21,0,0,0,250,0,0,35,93,"Fine Crossbow","Crossbow" ,0,0, 0,0,0,0},
{item_variety::Crossbow,0,0,4,0,0,0,21,0,0,0,3000,0,1,20,93,"Magic Crossbow","Crossbow" ,0,0, 0,0,0,0},
{item_variety::Bolts,14,0,0,0,12,0,86,0,16,0,3,0,0,1,30,"Bolts","Bolts" ,0,0, 0,0,0,0},
{item_variety::Bolts,14,0,2,0,10,0,86,0,17,0,40,0,0,1,30,"Iron Bolts","Bolts" ,0,0, 0,0,0,0}, // 120
{item_variety::Bolts,14,0,5,0,8,0,86,0,18,0,80,0,1,1,30,"Magic Bolts","Bolts" ,0,0, 0,0,0,0},
{item_variety::MissileWeapon,8, 0,0,0,0,0, 0,0,0,0, 20, 0,0, 25,0, "Leather Sling", "Sling" ,0,0, 0,0,0,0},
{item_variety::MissileWeapon,12, 0,3,0,0,0, 0,0,0,0, 3000, 0,1, 25,0, "Magic Sling", "Sling" ,0,0, 0,0,0,0},
{item_variety::Armor,1,0,0,0,0,0,14,0,0,0,4,1,0,40,50,"Leather Baldric","Leather Coat" ,0,0, 0,0,0,0}, 
{item_variety::Armor,2,0,0,0,0,0,14,0,0,0,10,1,0,50,51,"Leather Armor","Leather Armor" ,0,0, 0,0,0,0}, // 125
{item_variety::Armor,3,1,0,1,0,0,14,0,0,0,30,0,0,70,52,"Bronze Studded Armor","Studded Armor" ,0,0, 0,0,0,0},
{item_variety::Armor,5,2,0,1,0,0,15,0,0,0,75,0,0,90,53,"Bronze Chain Mail","Chain Mail" ,0,0, 0,0,0,0},
{item_variety::Armor,7,3,0,1,0,0,16,0,0,0,150,0,0,110,77,"Bronze Breastplate","Breastplate" ,0,0, 0,0,0,0},
{item_variety::Armor,9,4,0,1,0,0,16,0,0,0,350,0,0,130,54,"Bronze Plate","Plate" ,0,0, 0,0,0,0}, 
{item_variety::Armor,3,1,0,2,0,0,14,0,0,0,100,0,0,80,52,"Iron Studded Armor","Studded Armor" ,0,0, 0,0,0,0}, // 130
{item_variety::Armor,5,2,0,2,0,0,15,0,0,0,250,0,0,100,53,"Iron Chain Mail","Chain Mail" ,0,0, 0,0,0,0},
{item_variety::Armor,7,3,0,2,0,0,16,0,0,0,500,0,0,120,77,"Iron Breastplate","Breastplate" ,0,0, 0,0,0,0},
{item_variety::Armor,9,4,0,2,0,0,16,0,0,0,1200,0,0,140,54,"Iron Plate","Plate" ,0,0, 0,0,0,0},
{item_variety::Armor,3,1,0,3,0,0,14,0,0,0,200,0,0,80,52,"Steel Studded Armor","Studded Armor" ,0,0, 0,0,0,0},
{item_variety::Armor,5,2,0,3,0,0,15,0,0,0,400,0,0,100,53,"Steel Chain Mail","Chain Mail" ,0,0, 0,0,0,0}, // 135
{item_variety::Armor,7,3,0,3,0,0,16,0,0,0,800,0,0,120,77,"Steel Breastplate","Breastplate" ,0,0, 0,0,0,0},
{item_variety::Armor,9,4,0,3,0,0,16,0,0,0,2000,0,0,140,54,"Steel Plate","Plate" ,0,0, 0,0,0,0},
{item_variety::Armor,3,0,0,5,0,0,14,0,0,0,400,0,1,80,52,"Magic Studded Armor","Studded Armor" ,0,0, 0,0,0,0},
{item_variety::Armor,5,1,0,5,0,0,15,0,0,0,800,0,1,100,53,"Magic Chain Mail","Chain Mail" ,0,0, 0,0,0,0},
{item_variety::Armor,7,2,0,5,0,0,16,0,0,0,1600,0,1,120,77,"Magic Breastplate","Breastplate" ,0,0, 0,0,0,0}, //140
{item_variety::Armor,9,3,0,5,0,0,16,0,0,0,4000,0,1,140,54,"Magic Plate","Plate" ,0,0, 0,0,0,0},
{item_variety::Armor,5,0,0,4,0,0,15,0,0,0,1500,0,0,50,112,"Mithral Chain Mail","Chain Mail" ,0,0, 0,0,0,0},
{item_variety::Shield,1,1,0,0,0,0,45,0,0,0,2,0,0,20,46,"Crude Buckler","Buckler" ,0,0, 0,0,0,0},
{item_variety::Shield,1,1,0,1,0,0,45,0,0,0,15,0,0,20,46,"Bronze Buckler","Buckler" ,0,0, 0,0,0,0},
{item_variety::Shield,1,0,0,2,0,0,45,0,0,0,40,0,0,20,46,"Iron Buckler","Buckler" ,0,0, 0,0,0,0}, // 145
{item_variety::Shield,1,0,0,3,0,0,45,0,0,0,200,0,0,20,46,"Steel Buckler","Buckler" ,0,0, 0,0,0,0},
{item_variety::Shield,3,0,0,3,0,0,45,0,0,0,500,0,1,10,46,"Magic Buckler","Buckler" ,0,0, 0,0,0,0},
{item_variety::Shield,4,2,0,0,0,0,13,0,0,0,8,0,0,30,47,"Crude Shield","Shield" ,0,0, 0,0,0,0},
{item_variety::Shield,4,2,0,1,0,0,13,0,0,0,40,0,0,30,47,"Bronze Shield","Shield" ,0,0, 0,0,0,0},
{item_variety::Shield,4,1,0,2,0,0,13,0,0,0,100,0,0,30,47,"Iron Shield","Shield" ,0,0, 0,0,0,0},  // 150
{item_variety::Shield,4,1,0,3,0,0,13,0,0,0,400,0,0,30,47,"Steel Shield","Shield" ,0,0, 0,0,0,0},
{item_variety::Shield,6,1,0,3,0,0,13,0,0,0,1000,0,1,25,49,"Magic Shield","Shield" ,0,0, 0,0,0,0},
{item_variety::Shield,7,3,0,0,0,0,12,0,0,0,20,0,0,50,48,"Crude Large Shield","Shield" ,0,0, 0,0,0,0},
{item_variety::Shield,7,2,0,1,0,0,12,0,0,0,100,0,0,50,48,"Bronze Large Shield","Shield" ,0,0, 0,0,0,0},
{item_variety::Shield,7,2,0,2,0,0,12,0,0,0,250,0,0,50,84,"Iron Large Shield","Shield" ,0,0, 0,0,0,0}, // 155
{item_variety::Shield,7,2,0,3,0,0,12,0,0,0,700,0,0,50,48,"Steel Large Shield","Shield" ,0,0, 0,0,0,0},
{item_variety::Shield,8,1,0,3,0,0,12,0,0,0,2000,0,1,45,49,"Magic Large Shield","Shield" ,0,0, 0,0,0,0},
{item_variety::Helm,1,0,0,0,0,0,46,0,0,0,3,0,0,15,55,"Leather Helm","Helm" ,0,0, 0,0,0,0},
{item_variety::Helm,2,1,0,0,0,0,46,0,0,0,20,0,0,15,55,"Bronze Helm","Helm" ,0,0, 0,0,0,0},
{item_variety::Helm,3,1,0,0,0,0,46,0,0,0,40,0,0,15,55,"Iron Helm","Helm" ,0,0, 0,0,0,0},  // 160
{item_variety::Helm,4,1,0,0,0,0,46,0,0,0,100,0,0,15,55,"Steel Helm","Helm" ,0,0, 0,0,0,0},
{item_variety::Helm,5,1,0,0,0,0,46,0,0,0,250,0,1,15,56,"Magic Helm","Helm" ,0,0, 0,0,0,0},
{item_variety::Helm,3,1,0,0,0,0,47,0,0,0,12,0,0,25,57,"Leather Greathelm","Greathelm" ,0,0, 0,0,0,0},
{item_variety::Helm,4,2,0,0,0,0,47,0,0,0,40,0,0,25,57,"Bronze Greathelm","Greathelm" ,0,0, 0,0,0,0},
{item_variety::Helm,5,2,0,0,0,0,47,0,0,0,80,0,0,25,57,"Iron Greathelm","Greathelm" ,0,0, 0,0,0,0}, // 165
{item_variety::Helm,6,1,0,0,0,0,47,0,0,0,250,0,0,25,57,"Steel Greathelm","Greathelm" ,0,0, 0,0,0,0},
{item_variety::Helm,7,1,0,0,0,0,47,0,0,0,700,0,1,25,58,"Magic Greathelm","Greathelm" ,0,0, 0,0,0,0},
{item_variety::Gloves,1,0,0,0,0,0,48,0,0,0,10,0,0,7,59,"Leather Gloves","Gloves" ,0,0, 0,0,0,0},
{item_variety::Gloves,2,1,0,0,0,0,17,0,0,0,25,0,0,10,60,"Bronze Gauntlets","Gauntlets" ,0,0, 0,0,0,0},
{item_variety::Gloves,3,1,0,0,0,0,17,0,0,0,55,0,0,12,60,"Iron Gauntlets","Gauntlets" ,0,0, 0,0,0,0}, // 170
{item_variety::Gloves,4,1,0,0,0,0,17,0,0,0,150,0,0,12,60,"Steel Gauntlets","Gauntlets" ,0,0, 0,0,0,0},
{item_variety::Gloves,5,1,0,0,0,0,17,0,0,0,400,0,1,10,60,"Magic Gauntlets","Gauntlets" ,0,0, 0,0,0,0},
{item_variety::Boots,1,0,0,0,0,0,91,0,0,0,10,0,0,8,61,"Boots","Boots" ,0,0, 0,0,0,0},
{item_variety::Boots,2,0,0,0,0,0,91,0,0,0,50,0,0,12,62,"Steel-Toed Boots","Boots" ,0,0, 0,0,0,0},
{item_variety::Tool,0,0,0,0,4,0,36,11,14,0,8,0,0,1,27,"Lockpicks","Lockpicks" ,0,0, 0,3,0,161},// 175
{item_variety::WeaponPoison,2,0,0,0,1,0,55,8,31,0,15,0,0,3,40,"Weak Poison","Poison" ,0,0, 0,2,0,70},
{item_variety::WeaponPoison,4,0,0,0,1,0,55,8,32,0,50,0,0,3,40,"Medium Poison","Poison" ,0,0, 0,4,0,70},
{item_variety::WeaponPoison,6,0,0,0,1,0,55,8,33,0,150,0,0,3,40,"Strong Poison","Poison" ,0,0, 0,6,0,70},
{item_variety::WeaponPoison,8,0,0,0,1,0,55,8,34,0,400,0,0,3,40,"Killer Poison","Poison" ,0,0, 0,8,0,70},
{item_variety::GemStoneEtc,0,0,0,0,0,0,77,0,0,0,350,0,0,1,114,"Ruby","Gem" ,0,0, 0,0,0,0}, // 180
{item_variety::GemStoneEtc,0,0,0,0,1,0,76,17,19,0,250,0,0,1,31,"Sapphire","Gem" ,0,0, 0,0,0,158},
{item_variety::Tool,2,0,0,0,4,0,36,11,29,0,40,0,0,1,27,"Fine Lockpicks","Lockpicks" ,0,0, 0,5,0,161}, 
{item_variety::Tool,4,0,0,0,4,0,36,11,30,0,90,0,1,1,28,"Magic Lockpicks","Lockpicks" ,0,0, 0,8,0,161},
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,58,10,0,0,300,0,1,1,117,"Orb of Sight","Orb" ,0,0, 0,0,0,131},
{item_variety::None,0, 0,0,0,0,0, 0,0,0,0, 0, 0,0, 0,0, "Smoky Crystal", "Crystal" ,0,0, 0,0,0,159}, // 185
{item_variety::PotionOrMagicItem,0,0,0,0,1,0,57,41,0,0,200,0,1,1,118,"Piercing Crystal","Crystal" ,0,0, 0,5,0,132},
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,58,28,0,0,200,0,1,1,117,"Mist Globe","Orb" ,0,0, 1,8,0,85}, //h
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,58,24,0,0,200,0,1,1,117,"Mist Globe","Orb" ,0,0, 0,4,0,86}, // h
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,58,40,0,0,200,0,1,1,117,"Mist Globe","Orb" ,0,0, 2,9,0,84}, // h
{item_variety::PotionOrMagicItem,0,0,0,0,4,0,57,27,0,0,200,0,1,1,118,"Shielding Crystal","Crystal" ,0,0, 0,3,0,133},
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,58,85,0,0,100,0,1,1,117,"Goo Bomb","Orb" ,0,0, 0,7,0,127},
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,58,86,0,0,100,0,1,1,117,"Orb of Foul Vapors","Orb" ,0,0, 0,0,0,122},
{item_variety::PotionOrMagicItem,0,0,0,0,3,0,63,87,0,0,100,0,1,1,42,"Dust of Hiding","Dust" ,0,0, 2,6,0,78}, //195
{item_variety::PotionOrMagicItem,0,0,0,0,6,0,63,28,0,0,1,0,1,1,43,"Dust of Choking","Dust" ,0,0, 1,8,0,72},
{item_variety::PotionOrMagicItem,0,0,0,0,6,0,63,88,36,0,150,0,1,1,44,"Cleansing Powder","Dust" ,0,0, 0,9,0,77},
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,64,72,0,0,750,0,1,1,120,"Gold Statue","Figure" ,0,0, 0,2,0,119}, //m
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,64,73,0,0,200,0,1,1,120,"Ivory Bug","Figure" ,0,0, 0,2,0,119}, //m
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,64,76,0,0,70,0,1,1,120,"Ebony Lizard","Figure" ,0,0, 0,2,0,119}, // m
{item_variety::PotionOrMagicItem,0,0,0,0,5,0,65,89,0,0,100,0,1,1,121,"Horn of Warriors","Horn" ,0,0, 0,2,0,120},//m
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,33,5,0,0,50,0,1,1,16,"Scroll: Flame","Scroll" ,0,0, 0,7,0,110},
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,33,39,0,0,50,0,1,1,16,"Scroll: Slow","Scroll" ,0,0, 0,7,0,115},
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,33,19,0,0,100,0,1,1,17,"Scroll: Poison","Scroll" ,0,0, 0,7,0,124},
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,33,21,0,0,150,0,1,1,17,"Scroll: Stealth","Scroll" ,0,0, 0,7,0,91}, // 205
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,33,23,0,0,400,0,1,1,18,"Scroll: Kill","Scroll" ,0,0, 0,7,0,113},
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,33,38,0,0,200,0,1,1,18,"Scroll: Ice Bolt","Scroll" ,0,0, 0,7,0,114},
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,33,6,0,0,200,0,1,1,19,"Scroll: Fireball","Scroll" ,0,0, 0,7,0,111},
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,33,22,0,0,300,0,1,1,19,"Scroll: Firestorm","Scroll" ,0,0, 0,7,0,112},
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,33,90,0,0,500,0,1,1,19,"Scroll: Shockstorm","Scroll" ,0,0, 0,7,0,125}, // 210
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,33,37,0,0,600,0,1,1,16,"Scroll: Shockwave","Scroll" ,0,0, 0,7,0,116},
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,33,80,0,0,250,0,1,1,17,"Scroll: Charm","Scroll" ,0,0, 0,7,0,134},
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,33,36,0,0,300,0,1,1,18,"Scroll: Magic Res.","Scroll" ,0,0, 2,5,0,75},
{item_variety::ScrollOrMagicItem,0,0,0,0,1,0,64,91,0,0,100,0,1,1,120,"Alabaster Lizard","Figure" ,0,0, 0,0,0,119}, //m
{item_variety::Wand,0,0,0,0,6,0,34,45,0,0,150,0,1,1,21,"Prismatic Wand","Wand" ,0,0, 0,0,0,117}, 
{item_variety::Wand,0,0,0,0,6,0,34,44,0,0,100,0,1,1,22,"Crystal Wand","Wand" ,0,0, 0,0,0,118},
{item_variety::PotionOrMagicItem,0,0,0,0,1,1,30,31,0,0,2000,0,1,1,8,"Brew of Knowledge","Potion" ,0,0, 0,2,0,88},
{item_variety::PotionOrMagicItem,1,0,0,0,1,1,31,27,0,0,1200,0,1,1,9,"Brew of Ironskin","Potion" ,0,0, 0,4,0,74},
{item_variety::PotionOrMagicItem,0,0,0,0,1,1,32,115,0,0,900,0,1,1,10,"Brew of Battle","Potion" ,0,0, 0,8,0,71}, 
{item_variety::PotionOrMagicItem,0,0,0,0,1,1,30,63,0,0,500,0,1,1,11,"Brew of Lethe","Potion" ,0,0, 1,8,0,85}, // h
{item_variety::GemStoneEtc,0,0,0,0,0,0,56,0,39,0,700,0,0,1,116,"Diamond","Gem" ,0,0, 0,0,0,0},
{item_variety::GemStoneEtc,0,0,0,0,0,0,58,0,40,0,400,0,0,1,116,"Emerald","Gem" ,0,0, 0,0,0,0},// 225
{item_variety::Helm,3,4,0,0,0,0,46,14,0,0,10,0,1,14,56,"Cursed Helm","Helm" ,0,0, 0,0,0,0},
{item_variety::Helm,4,1,0,0,5,0,46,6,0,0,150,0,1,14,56,"Ruby Helm","Helm" ,0,0, 0,4,0,111},
{item_variety::Helm,4,1,0,0,5,0,46,19,0,0,120,0,1,14,56,"Emerald Helm","Helm" ,0,0, 0,6,0,124},
{item_variety::Helm,4,1,0,0,0,0,47,42,0,0,1400,0,1,14,56,"Helm of Speed","Helm" ,0,0, 0,5,0,55},
{item_variety::Helm,5,0,1,0,0,0,46,0,0,0,1200,0,1,14,56,"Runed Helm","Helm" ,0,0, 0,0,0,0}, // 230
{item_variety::GemStoneEtc,0,0,0,0,0,0,57,0,0,0,10,0,0,0,116,"Crystal","Crystal" ,0,0, 0,0,0,0},
{item_variety::GemStoneEtc,0,0,0,0,0,0,67,0,0,0,10,0,0,20,127, "Iron Bar", "Metal Bar" ,0,0, 0,0,0,0},
{item_variety::GemStoneEtc,0,0,0,0,0,0,67,0,0,0,100,0,0,20,127, "Silver Bar", "Metal Bar" ,0,0, 0,0,0,0},
{item_variety::GemStoneEtc,0,0,0,0,0,0,67,0,0,0,1000,0,0,18,127, "Platinum bar", "Metal Bar" ,0,0, 0,0,0,0},
{item_variety::Gloves,1,0,0,0,0,0,48,61,0,0,1000,0,1,7,123,"Nimble Gloves","Gloves" ,0,0, 0,6,0,42}, // 235
{item_variety::Gloves,3,1,0,0,0,0,17,96,0,0,1000,0,1,10,123,"Ogrish Gauntlets","Gauntlets" ,0,0, 0,4,0,43}, 
{item_variety::Gloves,3,1,0,0,0,0,17,97,0,0,3000,0,1,10,123,"Giantish Gauntlets","Gauntlets" ,0,0, 0,8,0,43}, 
{item_variety::Gloves,0,2,0,0,0,0,17,14,0,0,10,0,1,10,123,"Glue Gauntlets","Gauntlets" ,0,0, 0,0,0,0},
{item_variety::Gloves,1,0,0,0,0,0,48,49,0,0,800,0,1,7,123,"Aspskin Gloves","Gloves" ,0,0, 0,2,0,51},
{item_variety::Gloves,1,0,0,1,0,0,48,99,0,0,4000,0,1,7,123,"Micah's Gloves","Gloves" ,0,0, 0,8,0,40},
{item_variety::Boots,2,0,0,1,0,0,91,0,0,0,300,0,1,8,61,"Magic Boots","Boots" ,0,0, 0,0,0,0},
{item_variety::Boots,1,0,0,0,0,0,91,95,0,0,20,0,1,8,62,"Dancing Boots","Boots" ,0,0, 0,5,0,56}, //h
{item_variety::Boots,2,0,0,1,10,0,91,26,0,0,100,0,1,8,63,"Boots of Apollo","Boots" ,0,0, 0,5,0,73},
{item_variety::Boots,2,0,0,1,0,0,91,94,0,0,3000,0,1,8,62,"Boots of Speed","Boots" ,0,0, 0,5,0,55}, // 245
{item_variety::Boots,1,2,0,-3,0,0,91,14,0,0,10,0,1,8,63,"Cursed Boots","Boots" ,0,0, 0,0,0,0},
{item_variety::Shield,6,2,2,0,0,0,12,25,0,0,1000,0,1,30,49,"Crystal Shield","Shield" ,0,0, 0,0,0,49},
{item_variety::Shield,6,2,2,0,0,0,12,16,0,0,1500,0,1,30,49,"Iceshield","Shield" ,0,0, 0,0,0,33},
{item_variety::Shield,6,2,1,0,6,0,12,24,0,0,200,0,1,30,49,"Shield of Khar","Shield" ,0,0, 0,4,0,87}, 
{item_variety::Shield,6,2,1,0,6,0,12,18,0,0,200,0,1,30,49,"Shield of Klin","Shield" ,0,0, 0,6,0,71}, //250???
{item_variety::Shield,7,2,2,0,1,0,12,9,0,0,5000,0,1,30,49,"Lifeshield","Shield" ,0,0, 0,0,0,48},
{item_variety::Shield,6,2,1,0,5,0,12,90,0,0,150,0,1,30,49,"Shield of Klud","Shield" ,0,0, 0,0,0,0},//??
{item_variety::Shield,5,2,2,0,5,0,13,100,0,0,100,0,1,30,49,"Martyr's Shield","Shield" ,0,0, 0,4,0,80},
{item_variety::Shield,6,0,2,0,0,0,13,0,0,0,1500,0,1,30,49,"Runeshield","Shield" ,0,0, 0,0,0,0}, 
{item_variety::Shield,4,5,0,0,0,0,13,14,0,0,10,0,1,30,49,"Cursed Shield","Shield" ,0,0, 0,0,0,0}, //255
{item_variety::PotionOrMagicItem,3,0,0,0,1,1,30,4,0,0,350,0,1,5,8,"Strong Curing P.","Potion" ,0,0, 0,8,0,72}, 
{item_variety::PotionOrMagicItem,3,0,0,0,1,1,30,3,0,0,350,0,1,5,9,"Strong Healing P.","Potion" ,0,0, 0,8,0,87},
{item_variety::PotionOrMagicItem,3,0,0,0,1,1,30,20,0,0,600,0,1,5,10,"Strong Energy P.","Potion" ,0,0, 0,8,0,88}, 
{item_variety::PotionOrMagicItem,3,0,0,0,1,1,30,18,0,0,160,0,1,5,11,"Strong Strength P.","Potion" ,0,0, 0,8,0,71},
{item_variety::PotionOrMagicItem,3,0,0,0,1,1,30,26,0,0,200,0,1,5,12,"Strong Speed P.","Potion" ,0,0, 0,8,0,73}, //260
{item_variety::PotionOrMagicItem,3,0,0,0,1,1,30,27,0,0,600,0,1,5,14,"Strong Invuln. P.","Potion" ,0,0, 0,8,0,74},
{item_variety::PotionOrMagicItem,3,0,0,0,1,1,30,31,0,0,1600,0,1,5,15,"Strong Skill P.","Potion" ,0,0, 0,4,0,86},
{item_variety::PotionOrMagicItem,1,0,0,0,1,1,30,4,0,0,120,0,1,5,8,"Medium Curing P.","Potion" ,0,0, 0,5,0,72},
{item_variety::PotionOrMagicItem,1,0,0,0,1,1,30,3,0,0,180,0,1,5,9,"Medium Healing P.","Potion" ,0,0, 0,5,0,87}, //265
{item_variety::PotionOrMagicItem,1,0,0,0,1,1,30,20,0,0,300,0,1,5,10,"Medium Energy P.","Potion" ,0,0, 0,5,0,88},
{item_variety::PotionOrMagicItem,1,0,0,0,1,1,30,18,0,0,80,0,1,5,11,"Medium Strength P.","Potion" ,0,0, 0,5,0,71},
{item_variety::PotionOrMagicItem,1,0,0,0,1,1,30,26,0,0,100,0,1,5,12,"Medium Speed P.","Potion" ,0,0, 0,5,0,73}, 
{item_variety::PotionOrMagicItem,1,0,0,0,1,1,30,27,0,0,300,0,1,5,14,"Medium Invuln. P.","Potion" ,0,0, 0,5,0,74},//270
{item_variety::PotionOrMagicItem,1,0,0,0,1,1,30,31,0,0,800,0,1,5,15,"Medium Skill P.","Potion" ,0,0, 0,2,0,86},
{item_variety::PotionOrMagicItem,0,0,0,0,1,1,30,4,0,0,40,0,1,5,8,"Weak Curing P.","Potion" ,0,0, 0,2,0,72},
{item_variety::PotionOrMagicItem,0,0,0,0,1,1,30,3,0,0,60,0,1,5,9,"Weak Healing P.","Potion" ,0,0, 0,2,0,87}, 
{item_variety::PotionOrMagicItem,0,0,0,0,1,1,30,20,0,0,100,0,1,5,10,"Weak Energy P.","Potion" ,0,0, 0,2,0,88},
{item_variety::PotionOrMagicItem,0,0,0,0,1,1,30,18,0,0,40,0,1,5,11,"Weak Strength P.","Potion" ,0,0, 0,2,0,71},//275
{item_variety::PotionOrMagicItem,0,0,0,0,1,1,30,26,0,0,50,0,1,5,12,"Weak Speed P.","Potion" ,0,0, 0,2,0,73},
{item_variety::PotionOrMagicItem,0,0,0,0,1,1,30,27,0,0,150,0,1,5,14,"Weak Invuln. P.","Potion" ,0,0, 0,2,0,74},
{item_variety::PotionOrMagicItem,0,0,0,0,1,1,30,31,0,0,400,0,1,5,15,"Weak Skill P.","Potion" ,0,0, 0,1,0,86},
{item_variety::PotionOrMagicItem,0,0,0,0,1,1,30,24,0,0,400,0,1,5,8,"Ambrosia","Potion" ,0,0, 0,20,0,87},//280
{item_variety::PotionOrMagicItem,0,0,0,0,1,1,30,30,0,0,250,0,1,5,9,"Potion of Bliss","Potion" ,0,0, 0,7,0,84},
{item_variety::PotionOrMagicItem,0,0,0,0,1,1,30,28,0,0,10,0,1,5,10,"Poison Potion","Potion" ,0,0, 1,7,0,72},
{item_variety::PotionOrMagicItem,0,0,0,0,1,1,30,29,0,0,20,0,1,5,11,"Potion of Doom","Potion" ,0,0, 0,5,0,89},//h
{item_variety::PotionOrMagicItem,0,0,0,0,1,1,30,63,0,0,50,0,1,5,12,"Brew of Lethe","Potion" ,0,0, 1,8,0,85},//h
{item_variety::PotionOrMagicItem,0,0,0,0,1,1,30,79,0,0,200,0,1,5,13,"Potion of Clarity","Potion" ,0,0, 0,8,0,79},//285
{item_variety::ThrownMissile,4,0,2,0,1,0,28,65,0,0,250,0,1,1,5,"Dart of Returning","Darts" ,0,0, 0,0,0,170},
{item_variety::ThrownMissile,8,0,0,0,4,0,11,33,35,0,25,0,1,1,7,"Lightning Rods","Javelins" ,0,0, 0,20,0,171}, 
{item_variety::Arrows,9,0,1,0,1,0,27,65,0,0,1000,0,1,1,3,"Regenerating Arrows","Arrows" ,0,0, 0,0,0,170},
{item_variety::Arrows,9,0,2,0,5,0,27,34,0,0,150,0,1,1,3,"Arrows of Light","Arrows" ,0,0, 0,8,0,2},
{item_variety::Arrows,9,0,2,0,6,0,27,35,0,0,75,0,1,1,3,"Arrows of Life","Arrows" ,0,0, 0,8,0,3},//290
{item_variety::Arrows,9,0,1,0,6,0,27,33,0,0,30,0,1,1,3,"Burning Arrows","Arrows" ,0,0, 0,7,0,1}, 
{item_variety::Arrows,9,0,1,0,4,0,27,92,0,0,150,0,1,1,3,"Exploding Arrows","Arrows" ,0,0, 0,25,0,172},
{item_variety::Bow,0,0,3,5,0,0,10,0,0,0,2000,0,1,1,1,"Archer's Bow","Bow" ,0,0, 0,0,0,0},//h
{item_variety::Bow,0,0,0,0,0,0,10,93,0,0,200,0,1,1,1,"Bow of Kag","Bow" ,0,0, 0,0,0,11}, //h
{item_variety::ThrownMissile,4,0,3,0,6,0,28,32,0,0,18,0,1,1,5,"Poison Darts","Darts" ,0,0, 0,8,0,14},//295
{item_variety::Wand,0,0,0,0,8,0,34,19,0,0,80,0,1,8,21,"Wand of Venom","Wand" ,0,0, 0,0,0,124}, 
{item_variety::Wand,0,0,0,0,6,0,34,6,0,0,120,0,1,8,22,"Wand of Fireballs","Wand" ,0,0, 0,0,0,111},
{item_variety::Wand,0,0,0,0,8,0,34,5,0,0,60,0,1,8,23,"Wand of Flame","Wand" ,0,0, 0,0,0,110},
{item_variety::Wand,0,0,0,0,6,0,34,43,0,0,40,0,1,8,24,"Wand of Carrunos","Wand" ,0,0, 0,0,0,128},//h
{item_variety::Wand,0,0,0,0,8,0,34,39,0,0,20,0,1,8,25,"Wand of Slow","Wand" ,0,0, 0,0,0,115},//300
{item_variety::Wand,0,0,0,0,4,0,34,23,0,0,400,0,1,8,21,"Wand of Death","Wand" ,0,0, 0,0,0,113},
{item_variety::Wand,0,0,0,0,6,0,34,80,0,0,80,0,1,8,22,"Wand of Charming","Wand" ,0,0, 0,0,0,134},
{item_variety::Wand,0,0,0,0,5,0,34,81,0,0,80,0,1,8,23,"Wand of Nullity","Wand" ,0,0, 0,0,0,135},//h
{item_variety::Wand,0,0,0,0,6,0,34,83,0,0,10,0,1,8,25,"Wand of Vorb","Wand" ,0,0, 1,10,0,87},//h
{item_variety::Wand,0,0,0,0,6,0,34,84,0,0,60,0,1,8,21,"Wand of Rats","Wand" ,0,0, 0,0,0,119}, // m
{item_variety::Ring,2,0,0,0,0,0,49,46,0,0,2500,0,1,1,65,"Gold Ring of Regen.","Ring" ,0,0, 0,9,0,50}, 
{item_variety::Ring,2,0,0,7,0,0,49,0,0,0,675,0,1,1,65,"Gold Ring of Prot.","Ring" ,0,0, 0,0,0,0},
{item_variety::Ring,2,0,0,0,0,0,49,70,0,0,540,0,1,1,65,"Gold Ring of Accuracy","Ring" ,0,0, 0,8,0,41},
{item_variety::Ring,2,0,0,0,0,0,49,101,0,0,1400,0,1,1,65,"Gold Ring of Skill","Ring" ,0,0, 0,8,0,37}, //310
{item_variety::Ring,0,4,0,0,0,0,49,14,0,0,50,0,1,1,65,"Gold Ring of Weight","Ring" ,0,0, 0,8,0,45}, //h 
{item_variety::Ring,0,0,0,0,0,0,49,0,0,0,270,0,0,1,65,"Gold Ring","Ring" ,0,0, 0,0,0,0}, 
{item_variety::Ring,2,0,0,0,3,0,49,71,0,0,180,0,1,1,65,"Gold Serpent Ring","Ring" ,0,0, 0,0,0,120},//m
{item_variety::Ring,1,0,0,0,0,0,49,46,0,0,1000,0,1,1,66,"Silver Ring of Regen.","Ring" ,0,0, 0,5,0,50},
{item_variety::Ring,1,0,0,5,0,0,49,0,0,0,300,0,1,1,66,"Silver Ring of Prot.","Ring" ,0,0, 0,5,0,0}, //315
{item_variety::Ring,1,0,0,0,0,0,49,70,0,0,240,0,1,1,66,"Silver Ring of Accuracy","Ring" ,0,0, 0,5,0,41},
{item_variety::Ring,1,0,0,0,0,0,49,101,0,0,600,0,1,1,66,"Silver Ring of Skill","Ring" ,0,0, 0,5,0,37}, 
{item_variety::Ring,0,3,0,0,0,0,49,14,0,0,20,0,1,1,66,"Silver Ring of Weight","Ring" ,0,0, 0,5,0,45},//h
{item_variety::Ring,0,0,0,0,0,0,49,0,0,0,100,0,0,1,66,"Silver Ring","Ring" ,0,0, 0,0,0,0}, 
{item_variety::Ring,1,0,0,0,7,0,49,71,0,0,40,0,1,1,66,"Silver Serpent Ring","Ring" ,0,0, 0,0,0,119},//m
{item_variety::Ring,0,0,0,0,0,0,49,46,0,0,500,0,1,1,67,"Bronze Ring of Regen.","Ring" ,0,0, 0,2,0,50},
{item_variety::Ring,0,0,0,3,0,0,49,0,0,0,150,0,1,1,67,"Bronze Ring of Prot.","Ring" ,0,0, 0,0,0,0}, 
{item_variety::Ring,0,0,0,0,0,0,49,70,0,0,120,0,1,1,67,"Bronze Ring of Accuracy","Ring" ,0,0, 0,2,0,41},
{item_variety::Ring,0,0,0,0,0,0,49,101,0,0,300,0,1,1,67,"Bronze Ring of Skill","Ring" ,0,0, 0,2,0,37},
{item_variety::Ring,0,2,0,0,0,0,49,14,0,0,10,0,1,1,67,"Bronze Ring of Weight","Ring" ,0,0, 0,2,0,45},//h
{item_variety::Ring,0,0,0,0,0,0,49,0,0,0,60,0,0,1,67,"Bronze Ring","Ring" ,0,0, 0,0,0,0},
{item_variety::Ring,0,0,0,0,3,0,49,71,0,0,40,0,1,1,67,"Bronze Serpent Ring","Ring" ,0,0, 0,0,0,119}, //m 
{item_variety::Ring,0,0,0,0,0,0,49,66,0,0,1000,0,1,1,68,"Ring of Warmth","Ring" ,0,0, 0,4,0,33},
{item_variety::Ring,0,0,0,0,0,0,49,16,0,0,1200,0,1,1,68,"Ring of Fire Res.","Ring" ,0,0, 0,4,0,32},
{item_variety::Ring,0,0,0,0,0,0,49,74,0,0,3000,0,1,1,68,"Ring of Speed","Ring" ,0,0, 0,4,0,55},//330
{item_variety::Ring,0,0,0,0,0,0,49,75,0,0,800,0,1,1,69,"Ring of Will","Ring" ,0,0, 0,7,0,53}, 
{item_variety::Ring,0,0,0,0,0,0,49,67,0,0,750,0,1,1,69,"Aescal's Ring","Ring" ,0,0, 0,2,0,94},
{item_variety::Necklace,0,0,0,0,0,0,54,48,0,0,600,0,1,1,71,"Silver Ankh","Necklace" ,0,0, 0,6,0,57}, 
{item_variety::Necklace,0,0,0,0,0,0,53,49,0,0,500,0,1,3,72,"Amulet of the Cobra","Necklace" ,0,0, 0,3,0,51},
{item_variety::Necklace,0,0,0,0,0,0,54,16,0,0,1200,0,1,3,124,"Ruby Charm","Necklace" ,0,0, 0,5,0,32},//335
{item_variety::Necklace,0,0,0,0,0,0,53,2,0,0,1000,0,1,3,70,"Onyx Charm","Necklace" ,0,0, 0,5,0,35},
{item_variety::Necklace,0,0,0,0,0,0,53,47,0,0,750,0,1,3,72,"Fang Necklace","Necklace" ,0,0, 0,4,0,46},
{item_variety::Necklace,0,0,0,0,6,0,52,22,0,0,150,0,1,3,125,"Fire Orb Necklace","Necklace" ,0,0, 0,7,0,111},
{item_variety::Necklace,0,0,0,0,0,0,54,0,0,0,1000,0,0,3,70,"Diamond Necklace","Necklace" ,0,0, 0,0,0,0},
{item_variety::Necklace,0,0,0,0,0,0,52,0,0,0,400,0,0,3,70,"Gold necklace","Necklace" ,0,0, 0,0,0,0},//340
{item_variety::Necklace,0,0,0,0,1,0,52,9,0,0,2000,0,1,3,70,"Lifesaver Amulet","Necklace" ,0,0, 0,0,0,48},
{item_variety::Necklace,0,0,0,0,5,0,53,76,0,0,80,0,1,3,72,"Scale Necklace","Necklace" ,0,0, 0,0,0,119}, //m
{item_variety::Necklace,0,0,0,0,0,0,54,77,0,0,600,0,1,3,71,"Amber Periapt","Necklace" ,0,0, 0,0,0,34},
{item_variety::Necklace,0,0,0,-4,0,0,53,14,0,0,10,0,1,3,72,"Feldspar Charm","Necklace" ,0,0, 0,0,0,0}, //345
{item_variety::Necklace,0,0,0,4,0,0,52,0,0,0,700,0,1,3,71,"Ivory Charm","Necklace" ,0,0, 0,0,0,0},
{item_variety::Necklace,0,0,0,0,0,0,54,0,0,0,150,0,0,3,71,"Silver necklace","Necklace" ,0,0, 0,0,0,0},
{item_variety::GemStoneEtc,0,0,0,0,0,0,67,110,0,0,200,0,0,20,127, "Uranium bar", "Metal Bar" ,0,0, 0,3,0,52},
{item_variety::GemStoneEtc,0,0,0,0,0,0,92,0,0,0,750,0,0,20,127, "Gold bar", "Metal Bar" ,0,0, 0,0,0,0},
{item_variety::PotionOrMagicItem,0, 0,0,0,1,0, 30,59,0,0, 120, 1,1, 3,97, "Graymold Salve", "Salve" ,0,0, 2,3,0,77}, // 350
{item_variety::GemStoneEtc,0, 0,0,0,1,0, 31,13,0,0, 200, 1,1, 3,97, "Ressurection Balm", "Balm" ,0,0, 0,0,0,160},
{item_variety::GemStoneEtc,0, 0,0,0,0,0, 71,0,0,0, 400, 0,0, 5,135, "Rough Diamond", "Rock" ,0,0, 0,0,0,0},
{item_variety::GemStoneEtc,3, 0,0,0,5,0, 71,20,0,0, 150, 0,1, 5,135, "Power Geode", "Rock" ,0,0, 0,3,0,88},
{item_variety::GemStoneEtc,0, 0,0,0,9,0, 72,5,0,0, 80, 0,1, 5,135, "Firestone", "Rock" ,0,0, 0,3,0,110},
{item_variety::OneHandedWeapon,4, 0,6,0,0,1, 25,0,0,0, 800, 0,1, 5,110, "Ravage Knife", "Knife" ,0,0, 0,0,0,0}, // 355
{item_variety::OneHandedWeapon,6, 0,3,0,0,1, 25,32,0,0, 1000, 0,1, 5,110, "Assassin's Knife", "Knife" ,0,0, 0,8,0,14},
{item_variety::OneHandedWeapon,13, 0,3,0,0,1, 7,32,0,0, 5000, 0,1, 30,94, "Alien Blade", "Waveblade" ,0,0, 0,3,0,14},
{item_variety::TwoHandedWeapon,18, 0,5,0,0,1, 1,50,0,0, 8000, 0,1, 80,136, "Demonslayer", "Greatsword" ,0,0, 0,8,0,2},
{item_variety::OneHandedWeapon,12, 0,2,0,0,1, 0,131,0,0, 4000, 0,1, 30,86, "Wyrmsbane", "Broadsword" ,0,0, 0,6,0,4},
{item_variety::OneHandedWeapon,10, 0,1,0,0,1, 0,33,0,0, 1200, 0,1, 30,138, "Flaming Sword", "Red Sword" ,0,0, 0,5,0,1}, // 360
{item_variety::OneHandedWeapon,10, 0,3,0,0,2, 2,51,0,0, 4000, 0,1, 30,84, "Smite", "Hammer" ,0,0, 0,8,0,5},
{item_variety::OneHandedWeapon,10, 0,2,0,0,2, 2,35,0,0, 1250, 0,1, 40,81, "Mace of Disruption", "Mace" ,0,0, 0,7,0,3},
{item_variety::GemStoneEtc,0, 0,0,0,0,0, 72,129,0,0, 100, 0,1, 5,29, "Lodestone", "Rock" ,0,0, 0,4,0,45}, // h
{item_variety::GemStoneEtc,0, 0,0,0,0,0, 71,117,0,0, 400, 0,1, 5,29, "Airy Stone", "Rock" ,0,0, 0,4,0,44},
{item_variety::ScrollOrMagicItem,0, 0,0,0,4,0, 63,121,41,0, 40, 0,1, 2,42, "Basic Powder", "Powder" ,0,0, 2,8,0,83},
{item_variety::ScrollOrMagicItem,0, 0,0,0,2,0, 63,63,42,0, 30, 0,1, 2,43, "Powder of Lethe", "Powder" ,0,0, 3,4,0,85}, //h
{item_variety::PotionOrMagicItem,0, 0,0,0,1,0, 30,130,0,0, 30, 0,1, 4,14, "Potion of Paralysis", "Potion" ,0,0, 1,10,0,82}, // h
{item_variety::Ring,0, 0,0,0,0,0, 49,120,0,0, 1000, 0,1, 2,69, "Ring of Free Action", "Ring" ,0,0, 0,7,0,54},
{item_variety::Ring,0, 0,0,0,0,0, 49,127,0,0, 4000, 0,1, 2,68, "Ring of Resistance", "Ring" ,0,0, 0,8,0,31},
{item_variety::Necklace,0, 0,0,0,0,0, 53,122,0,0, 500, 0,1, 4,72, "Basic Necklace", "Necklace" ,0,0, 0,4,0,36},
{item_variety::Wand,0, 0,0,0,5,0, 34,125,0,0, 170, 0,1, 6,24, "Wand of Paralysis", "Wand" ,0,0, 0,6,0,126},
{item_variety::Wand,0, 0,0,0,7,0, 35,124,0,0, 100, 0,1, 7,25, "Wand of Acid", "Wand" ,0,0, 0,7,0,121}, // 375
{item_variety::Helm,4, 0,0,1,0,0, 46,118,0,0, 750, 0,1, 20,56, "Helm of Alertness", "Helm" ,0,0, 0,3,0,54},
{item_variety::Shield,6, 1,0,2,6,0, 13,123,0,0, 250, 0,1, 40,49, "Shield of Kron", "Shield" ,0,0, 0,0,0,123},
{item_variety::OneHandedWeapon,8, 0,0,0,8,1, 0,5,0,0, 50, 0,1, 20,111, "Boltblade", "Short Sword" ,0,0, 0,3,0,110}, // 380
{item_variety::Shield,8, 0,0,3,0,2, 2,0,0,0, 400, 0,1, 30,81, "Shield Mace", "Mace" ,0,0, 0,0,0,0},
{item_variety::Ring,2, 0,0,0,5,0, 49,26,0,0, 120, 0,1, 4,66, "Quicksilver Band", "Ring" ,0,0, 0,3,0,73},
{item_variety::GemStoneEtc,0, 0,0,0,3,0, 39,52,0,0, 10, 1,0, 1,128, "Holly", "Holly" ,0,0, 0,0,0,150},
{item_variety::GemStoneEtc,0, 0,0,0,3,0, 40,53,0,0, 15, 1,0, 1,129, "Comfrey Root", "Comfrey Root" ,0,0, 0,0,0,151}, // 385
{item_variety::GemStoneEtc,0, 0,0,0,3,0, 41,54,0,0, 30, 1,0, 1,130, "Glowing Nettle", "Glowing Nettle" ,0,0, 0,0,0,152},
{item_variety::GemStoneEtc,0, 0,0,0,3,0, 39,55,0,0, 50, 1,0, 1,131, "Wormgrass", "Wormgrass" ,0,0, 0,0,0,153},
{item_variety::GemStoneEtc,0, 0,0,0,3,0, 40,53,0,0, 70, 1,0, 1,129, "Asptongue Mold", "Asptongue Mold" ,0,0, 0,0,0,154}, // 385
{item_variety::GemStoneEtc,0, 0,0,0,2,0, 40,58,0,0, 90, 1,0, 1,132, "Graymold", "Graymold" ,0,0, 0,0,0,156},
{item_variety::GemStoneEtc,0, 0,0,0,2,0, 41,59,0,0, 120, 1,0, 1,133, "Ember Flowers", "Ember Flowers" ,0,0, 0,0,0,155},
{item_variety::GemStoneEtc,0, 0,0,0,2,0, 40,56,0,0, 400, 1,0, 1,134, "Mandrake Root", "Mandrake Root" ,0,0, 0,0,0,157}, // 390
{item_variety::GemStoneEtc,0, 0,0,0,1,0, 96,58,0,0, 40, 1,0, 1,144, "Asp Fangs", "Asp Fangs" ,0,0, 0,0,0,154},
{item_variety::OneHandedWeapon,3, 0,0,0,0,1, 3,14,0,0, 10, 0,1, 20,82, "Cursed Axe", "Axe" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,6, 0,0,0,0,1, 0,14,0,0, 10, 0,1, 25,86, "Cursed Broadsword", "Broadsword" ,0,0, 0,0,0,0},// 395
{item_variety::TwoHandedWeapon,8, 0,0,0,0,1, 1,14,0,0, 10, 0,1, 45,90, "Cursed Greatsword", "Greatsword" ,0,0, 0,0,0,0}, 
{item_variety::OneHandedWeapon,1, 0,0,0,0,1, 25,14,0,0, 10, 0,1, 8,110, "Cursed Knife", "Knife" ,0,0, 0,0,0,0},
{item_variety::Shield,1, 2,0,0,0,0, 13,14,0,0, 10, 0,1, 30,47, "Cursed Shield", "Shield" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,1, 0,0,0,0,2, 2,14,0,0, 10, 0,1, 20,81, "Cursed Mace", "Mace" ,0,0, 0,0,0,0},
{item_variety::Ring,0, 3,0,0,0,0, 49,14,0,0, 10, 0,1, 1,65, "Cursed Ring", "Ring" ,0,0, 0,0,0,0}, // 400
{item_variety::TwoHandedWeapon,8, 0,0,0,0,3, 5,14,0,0, 10, 0,1, 50,89, "Cursed Halberd", "Halberd" ,0,0, 0,0,0,0},
{item_variety::GemStoneEtc,0, 0,0,0,0,0, 71,0,46,0, 8, 1,0, 3,146, "Silver Ore", "Ore" ,0,0, 0,0,0,0},
{item_variety::GemStoneEtc,0, 0,0,0,0,0, 72,0,47,0, 15, 1,0, 3,146, "Gold Ore", "Ore" ,0,0, 0,0,0,0},
{item_variety::OneHandedWeapon,10,0,6,0,0,1,0,0,0,0,3000,0,1,25,86,"Mithral Broadsword","Broadsword" ,0,0, 0,0,0,0},
};
