#!/bin/bash
NAME="CHEVALOBOT";
echo "$NAME"

line="";
CURR=0;


while true;do
  while [[ $line != "Joueur $NAME :"*"," ]]; do
    read -t 0.2 -r line;
    if [[ $line == *"manche"* ]];then
      CURR=0;
    elif [[ $line == *"Voulez vous rejouer une partie ?(y/n):"* ]];then
      echo "n";
      read -t 5 -r line;
      clear;
      echo "$line" >&2;
      exit 0;
    elif [[ ! -z "$line" ]];then
      echo  -e "$line" >&2;
    fi
  done;
  # echo $(echo -n "${line}" | cut -d ':' -f2 | tr  ',' ' ') >&2;
  ARRAY="";
  for el in $(echo -n $(echo -n "${line}" | cut -d ':' -f2 | tr  ',' ' '));do
    ARRAY="$ARRAY $el";
  done;
  #sort array
  ARRAY=$(echo -n "${ARRAY}" | tr " " "\n" | sort -n | tr "\n" " ");
  # echo "ARRAY=${ARRAY}" >&2;

  i=$(echo -n "$ARRAY" | cut -d ' ' -f 2);
  MAGIE=$(($(echo -n "$i" | cut -d '(' -f 1)));
  SLEEP_TIME=$(($MAGIE-$CURR));
  CURR=$MAGIE;
  SLEEP_TIME=$(echo -n $SLEEP_TIME | awk '{}{print $1/10}');
  sleep "${SLEEP_TIME}";
  echo "$(echo -n $i | cut -d '(' -f 2 | cut -d ')' -f 1)";
  line="";
done;
