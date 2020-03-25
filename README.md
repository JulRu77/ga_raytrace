# ga_raytrace 

Prototype de rendu par "lancer de rayons" via "ga".

## Requis

- g++
- OpenMP

## Dependances

- C3GA : https://github.com/vincentnozick/garamon
Note: modifier le fichier CmakeList.txt pour indiquer l'empacement des includes et librairies c3ga

## Compilation
```
mkdir build
cd build
cmake ..
make -j
```

## Lancement

./ga_view spheres limit_x_y max_depth render_res