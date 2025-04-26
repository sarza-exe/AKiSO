#!/bin/bash

# Nagłówki
(echo -e "PPID\tPID\tCOMM\tSTATE\tTTY\tRSS\tPGID\tSID\tOPEN_FILES"

# Przechodzimy przez wszystkie PID w /proc
for pid in $(ls /proc | grep '^[0-9]'); do
    # Sprawdzamy, czy /proc/$pid/stat i /proc/$pid/fd istnieją
    if [[ -f /proc/$pid/stat && -d /proc/$pid/fd ]]; then

        # Wczytujemy dane z /proc/PID/stat
        ppid=$(awk '{print $4}' /proc/$pid/stat)
        comm=$(awk '{print $2}' /proc/$pid/stat)
        #usuwamy nawiasy z comm. Zamień nawiasy na pusty ciąg
        comm="${comm//[()]/}"
        state=$(awk '{print $3}' /proc/$pid/stat)
        tty=$(awk '{print $7}' /proc/$pid/stat)
        rss=$(awk '{print $24}' /proc/$pid/stat)
        pgid=$(awk '{print $5}' /proc/$pid/stat)
        sid=$(awk '{print $6}' /proc/$pid/stat)


        # Liczymy ilość otwartych plików w katalogu /proc/$pid/fd. wc -l liczy liczbę linii
        open_files=$(ls /proc/$pid/fd | wc -l)

        # Ustawia wartości domyślne jeśli rss albo tty mają wartość null
        rss=${rss:-0}
        tty=${tty:-'?'}
        ppid=${ppid:-'?'}

        # Wypisujemy dane w odpowiednim formacie
        echo -e "$ppid\t$pid\t$comm\t$state\t$tty\t$rss\t$pgid\t$sid\t$open_files"
    fi
done) | sort -k2 -n | column -t -s $'\t'