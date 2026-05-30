

Tämä projekti on tehty harjoitukseksi graafiseen koodaukseen, pelimoottoreihin, reaaliaikaiseen renderöintiin (20+ FPS) sekä raycastingin opetteluun c++ käyttämällä. OpenGL:ää käyttämällä ikkuna on jaettu kahteen osaan, joissa on generoitu karttanäkymä ja raycast-näkymä testi ympyrästä.

Vasemmalla puolella nähdään projektissa generoitu kartta ja pelaaja. Kartta koostuu 21 × 21 -kokoisesta matriisista, jossa on määritetty seinät, polut, alku ja loppu. Matriisista tehdään vertex-taulukko, jossa on asetettu jokaisen kartan ruudun kulmapisteet ikkunassa, jotka yhdistetään GPU:ssa kokonaiseksi kartaksi.

Pelaajan pisteestä generoidaan säteet havainnollistamaan pelaajan näkölinjaa. Pelaajan näkökentästä lisätään askeleittain säteitä kunnes ne löytävät seinän kartasta. Oikealla puolella ikkunaa on pseudo-3D-pelinäkymä pelaajan näkykulmasta.
```
Koodi repon kansion selitys:
/
├── Shaders/                 # Shader OpenGl koodi (sightline, pelaaja, maze kartta GPU puolelle)
├── glm/                     # GLM vec3 ja matrix matikka grafiikka lib
├── include/                 # Project header 
├── lib/                     # libs and DLLs
├── Game.cpp / Game.h        # Pelilogiikka + liikkuminen, pelaajan fov generointi ja raytrace
├── Maze.cpp / Maze.h        # Maze kartta generoiminen ja tilemap:in värien asettaminen
├── Renderer.cpp / Renderer.h# VAO ja VBO kartalle, fov ja renderöinti näkymä ikkunalle + puolitus
├── main.cpp                 # fps ja peli moottori
```


Demo video projektista:

https://github.com/user-attachments/assets/a32b6e9b-ccd5-4fa9-b7cf-56352aa65ab1





