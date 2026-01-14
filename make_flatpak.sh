flatpak-builder --user --install --force-clean build org.noise_planet.i-simpa.yml

# run it
# flatpak run org.noise_planet.i-simpa

# make flatpak bundle
# flatpak build-bundle .flatpak-builder/cache isimpa.flatpak org.noise_planet.i-simpa --runtime-repo=https://flathub.org/repo/flathub.flatpakrepo

# make independent flapak for offline install
# flatpak create-usb --user ./offline org.noise_planet.i-simpa