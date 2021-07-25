[TOC]

------

# 1. Add to your project

​	Add `SwitchIf/*.h` and `SwitchIf/*.hpp` source to your project;

​	`main.cpp`is testfile;

# 2. What's this

​	This is a branch statement that simulates `if` and `switch` but is used for multiple combination cases.



# 3. Reference

## 3.0 Global function

```C++
bool no() { return false; }
bool yes() { return true; }

void all_yes() { std::cout << "all yes" << std::endl; }
void all_no(){std::cout << "all no" << std::endl;}
void yes_no(){std::cout << "yes no" << std::endl;}
void no_yes(){std::cout << "no yes" << std::endl;}
void yes_and_no(){std::cout << "yes and no" << std::endl;}
void first_is_no() {std::cout << "first is no" << std::endl;}
void first_is_yes() { std::cout << "first is yes" << std::endl; }
```

## 3.1 MultIf

```C++
//1. MultIf right-value-object
std::cout << "MultIf right-value-object:" << std::endl;
MultIf<bool, 2>(yes(), no()).
	ifdo({ true,true }, [] {all_yes(); }).
	ifdo({ false,false }, [] {all_no(); }).
	ifdo({ true,false }, [] {yes_no(); }).	//to paint
	ifdo({ false,true }, [] {no_yes(); });

MultIf<bool, 2>{yes(), no()}.
	ifdo({ true,true }, [] {all_yes(); }).
	ifdo({ false,false }, [] {all_no(); }).
	elsedo([] {yes_and_no();});	//to paint

//2. MultIf left-value-object
std::cout << "MultIf left-value-object:" << std::endl;
MultIf<bool, 2> lif;
lif.setExpression(no(), yes()).
	ifdo({ true,true }, [] {all_yes(); }).
	ifdo({ false,false }, [] {all_no(); }).
	ifdo({ true,false }, [] {yes_no(); }).
	ifdo({ false,true }, [] {no_yes(); }).	//to paint
	//left-value-object finish shoud be call end()/elsedo(lambda) to prevent the next judgment is not interrupted
	end();

lif.setExpression(yes(),yes()).	//set new expression will reset
	ifdo({ true,true }, [] {all_yes(); }).	//to paint
	ifdo({ false,false }, [] {all_no(); }).
	elsedo([] {yes_and_no(); });


//3. MultIf peculiarly-code
std::cout << "MultIf peculiarly-code:" << std::endl;
lif.setExpression(no(), no());	//remember reset
lif.ifdo({ true,true }, [] {all_yes(); });
lif.ifdo({ false,false }, [] {all_no(); });	//to paint
lif.elsedo([] {yes_and_no();});
```



## 3.2 MultSwitch (Strict and Loose Mode)

```C++
//4. MultSwitchStrict right-value-obj
std::cout << "MultSwitchStrict right-value-obj:" << std::endl;
MultSwitchStrict<bool, 2>(no(), yes()).
	//real args is {true,bool{}} -> {true,false}
	//Use the default construct to complete the parameters
	casedo({ true }, [] {yes_no(); }).
	casedo({ false,true }, [] {no_yes(); }).	//to paint
	casedo({ true,true }, [] {all_yes(); }).	//to paint
	defaultdo([] {all_no(); });	//to paint	

MultSwitchStrict<bool, 2>(no(), yes()).
	//real args is {true,bool{}} -> {true,false}
	//Use the default construct to complete the parameters
	casedo({ true }, [] {yes_no(); }, CaseMod::Break).
	casedo({ false,true }, [] {no_yes(); }, CaseMod::Break).	//to paint
	casedo({ true,true }, [] {all_yes(); }, CaseMod::Break).
	defaultdo([] {all_no(); });

//5. MultSwitchLoose right-value-obj
std::cout << "MultSwitchLoose right-value-obj:" << std::endl;
MultSwitchLoose<bool, 2>(yes(), no()).
	//{true} is included {true,true} and {true,false}
	//Loose Mode Matches only the first N of a given number
	casedo({ true }, [] {first_is_yes(); }).	//to paint
	casedo({ false,true }, [] {yes_no(); }).	//to paint
	defaultdo([] {all_no(); });	//to paint

MultSwitchLoose<bool, 2>(yes(), yes()).
	//{true} is included {true,true} and {true,false}
	//Loose Mode Matches only the first N of a given number
	casedo({ true }, [] {first_is_yes(); }, CaseMod::Break).	//to paint
	casedo({ false,true }, [] {yes_no(); }, CaseMod::Break).
	defaultdo([] {all_no(); });
```

> left-value-object

```C++
//6. MultSwitchStrict left-value-obj
std::cout << "MultSwitchStrict left-value-obj:" << std::endl;
MultSwitchStrict<bool, 2> mss;
mss.setExpression(no(), yes()).
	casedo({ true }, [] {yes_no(); }).
	casedo({ false,true }, [] {no_yes(); }).	//to paint
	casedo({ true,true }, [] {all_yes(); }).	//to paint
	defaultdo([] {all_no(); });	//to paint	

mss.setExpression(no(), yes()).
	casedo({ true }, [] {yes_no(); }, CaseMod::Break).
	casedo({ false,true }, [] {no_yes(); }, CaseMod::Break).	//to paint
	casedo({ true,true }, [] {all_yes(); }, CaseMod::Break).
	//finished should be call end() or default()
	end();




//7. MultSwitchLoose left-value-obj 
std::cout << "MultSwitchLoose left-value-obj:" << std::endl;
MultSwitchLoose<bool, 2> msl;
msl.setExpression(yes(), no()).
//{true} is included {true,true} and {true,false}
//Loose Mode Matches only the first N of a given number
casedo({ true }, [] {first_is_yes(); }).	//to paint
casedo({ false,true }, [] {yes_no(); }).	//to paint
defaultdo([] {all_no(); });	//to paint

//{true} is included {true,true} and {true,false}
//Loose Mode Matches only the first N of a given number
msl.setExpression(yes(), yes());
msl.casedo({ true }, [] {first_is_yes(); }, CaseMod::Break);	//to paint
msl.casedo({ false,true }, [] {yes_no(); }, CaseMod::Break);
msl.defaultdo([] {all_no(); });
```

> casedo:

```C++
//8. MultSwitchLoose and MultSwitchStrict casedo() step by step 
std::cout << "MultSwitchLoose and MultSwitchStrict casedo() step by step:" << std::endl;
//In the case of a few expressions,should be use MultSwitchStrict to reduce space footprint
MultSwitchStrict<int, 1>(5).
	//casedo({ArgsList},bool is_break = CaseMod::NotBreak) is casedo({ArgsList},Lambda = nullptr ,bool is_break = CaseMod::NotBreak)
	casedo({ 1 }).
	casedo({ 2 }).
	casedo({ 3 }, [] {std::cout << "x <=3 " << std::endl; }, CaseMod::Break).	//to print
	casedo({ 4 }).
	casedo({ 5 }).
	casedo({ 6 }, [] {std::cout << "3< x <=6" << std::endl; },CaseMod::Break).
	defaultdo([] {std::cout << "6 < x" << std::endl;});

//Into the switch
switch(x){
    case 1:
    case 2:
    case 3:
    	std::cout << "x <=3 " << std::endl;
       	 break;
    case 4:
    case 5:
    case 6:
        std::cout << "3< x <=6" << std::endl;
        break;
    default:
        std::cout << "6 < x" << std::endl;
}
```



