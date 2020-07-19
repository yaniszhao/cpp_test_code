#循环
#!/bin/bash
#---for---do---done---#
sum="0"
for i in $(seq 100); do	
	sum=`expr $sum + $i`
done
echo	"sum = $sum"
echo	"i = $i"	
#---while---do---done---#
sum="0"
i="1"
while 	[  $i -le 100 ]; do
	sum=`expr $sum + $i`
	i=`expr $i + 1`
done
echo	"sum = $sum"
echo	"i = $i"
#---until---do---done---#
sum="0"
i="1"
until	[ $i -gt 100 ]; do
	sum=`expr $sum + $i`
	i=`expr $i + 1`
done
echo	"sum = $sum"
echo	"i = $i"
	
#==================================================#

#选择
#/bin/bash
#---if-elif-else---#
NUM=`expr 12 + 24 / 3`
FILE="lop.sh"
if	[ -d $FILE ]; then 
		echo "there is a directory!"
	if [ $NUM -eq 24 ]; then
		echo "NUM is equal to 24!"
	fi
elif 	[ -f $FILE ]; then 
		echo "there is a file!"
	if [ $NUM -eq 24 ]; then
		echo "NUM is equal to 24!"
	fi
elif	[ $NUM -eq 24 ]; then
		echo "NUM is equal to 24!"
elif	[ ! $NUM -eq 24 ]&&[ ! -d $FILE ]&&[ ! -f $FILE ]; then
		echo "there is not a directory neither a number is 24!"
else	
	echo "ERROR!"	
fi
#---use true---#
if true; then
	echo "succeed!"
else
	echo "failed!"
fi
#---case-esac--#
echo "please read your namenumber!(0-1):"
read NAMENUM
case $NAMENUM in
	0)	NAME=yanis
		;;
	1)	NAME=zhaoyue
		;;
	[^0-1])	echo "only 0-1 !"
		exit 1 
		;;
	*)	echo "please input single number!"
		exit 1
		;;
esac
echo "please read your gender!(male or female):"
read GENDER
case $GENDER in
	male)	echo "hello $GENDER $NAME!"
		;;
	female)	echo "hello $GENDER $NAME!"
		;;
	*)	echo "please input male or female !"
		echo "try again !"
		exit 1
		;;
esac
#---select-do-case-esac-done---#
echo "you can choose only one follow fruit, and use number [1-5] !"
select FRUIT in "balana" "orange" "apple" "pear" "exit"
do
case $FRUIT in
	balana)	echo "you have chose balana !"
		break
		;;
	orange)	echo "you have chose orange !"
		break
		;;
	apple)	echo "you have chose apple !"
		break
		;;
	pear)	echo "you have chose pear !"
		break
		;;
	*|exit)	echo "you have chose exit or you have chose incorrectly !"
		echo "do you want to chose again? (y/n):  ";
		read ANSWER
		case $ANSWER in
			y|Y|yes|YES)
					echo "now, you can choose fruit again:"
					continue
					;;
			n|no|N|NO)	echo "you have chose exit !"	
					exit
					;;		
			*)		echo "this is a incorrect option, exit !"
					exit
					;;
		esac
		;;
esac
done