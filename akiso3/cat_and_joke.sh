#!/bin/bash

image_file="cat_image.jpg"

# Pobiernie url kota
# -q - Quiet mode, -O - Directs the output to stdout. jq -r tekst bez cytatow
cat_image_url=$(wget -qO- https://api.thecatapi.com/v1/images/search | jq -r '.[0].url')

# -z sprawdza czy ciąg znaków jest pusty
if [[ -z "$cat_image_url" ]]; then
    echo "Failed to fetch cat image URL."
    exit 1
fi

# Pobieranie zdjęcia kota
wget -q $cat_image_url -O "$image_file"

# Zwróc prawdę jeśli plik nie istnieje
if [[ ! -f "$image_file" ]]; then
    echo "Failed to download the cat image."
    exit 1
fi

# Wyświetlamy zdjęcie kotka używając catimg
# Sprawdza czy catimg jest dostępne w systemie.
# Przekierowuje dane wyjsciowe do czarnej dziury
if command -v catimg &> /dev/null; then
    catimg "$image_file"
else
    echo "catimg is not installed."
fi

# Pobieranie żartu
norris_joke=$(wget -qO- https://api.chucknorris.io/jokes/random | jq -r '.value')

if [[ -z "$norris_joke" ]]; then
    echo "Failed to fetch a great joke."
    exit 1
fi

# Wyświetlamy żart
echo -e "\nRandom Chuck Norris joke:\n$norris_joke"

# Usuwamy niepotrzebnego już nam kota
rm -f "$image_file"