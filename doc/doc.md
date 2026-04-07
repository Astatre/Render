# Documentation pour main.cpp

## Vue d'ensemble

Ce programme C++ est une application OpenGL de base qui utilise GLFW pour la gestion des fenêtres et GLAD pour le chargement des fonctions OpenGL. Il affiche un rectangle orange dans une fenêtre en utilisant des shaders vertex et fragment.

## Dépendances

- **GLFW** : Bibliothèque pour la création de fenêtres et la gestion des entrées
- **GLAD** : Chargeur de fonctions OpenGL
- **OpenGL** : API graphique pour le rendu 3D

## Structure du projet

```
/
├── Makefile          # Script de compilation
├── include/          # En-têtes GLAD
│   ├── glad/
│   │   ├── glad.h
│   └── KHR/
│       └── khrplatform.h
└── src/
    ├── main.cpp      # Code principal
    ├── glad.c        # Implémentation GLAD
    ├── vertexShader.glsl   # Shader vertex
    └── fragmentShader.glsl # Shader fragment
```

## Compilation et exécution

### Compilation
```bash
make
```

### Exécution
```bash
make run
# ou
./app
```

### Nettoyage
```bash
make clean
```

## Description du code

### Initialisation

1. **Initialisation GLFW** : `glfwInit()` crée le contexte GLFW
2. **Création de la fenêtre** : `glfwCreateWindow(800, 600, "Test", NULL, NULL)` crée une fenêtre de 800x600 pixels
3. **Contexte OpenGL** : `glfwMakeContextCurrent(window)` rend le contexte OpenGL actif
4. **Initialisation GLAD** : `gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)` charge les fonctions OpenGL

### Shaders

Le programme utilise deux shaders :

#### Vertex Shader
```glsl
#version 330 core
layout (location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```
- Prend les positions des sommets en entrée
- Définit la position finale en coordonnées homogènes

#### Fragment Shader
```glsl
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
```
- Définit la couleur de sortie (orange)

### Buffers et données

- **VBO (Vertex Buffer Object)** : Stocke les coordonnées des sommets
- **EBO (Element Buffer Object)** : Stocke les indices pour dessiner les triangles
- **VAO (Vertex Array Object)** : Configure les attributs des sommets

Les sommets définissent un rectangle :
```
(0.5, 0.5, 0.0)   (-0.5, 0.5, 0.0)
     +-----------------+
     |                 |
     |                 |
     +-----------------+
(-0.5, -0.5, 0.0)  (0.5, -0.5, 0.0)
```

### Boucle de rendu

La boucle principale :
1. Efface l'écran avec une couleur de fond (vert-bleu)
2. Traite les entrées (touche Échap pour quitter)
3. Dessine les triangles en utilisant `glDrawElements`
4. Met à jour la fenêtre

### Gestion des entrées

- **framebuffer_size_callback** : Ajuste la vue OpenGL lors du redimensionnement
- **processInput** : Vérifie la touche Échap pour fermer l'application

## Fonctions principales

### `framebuffer_size_callback(GLFWwindow* window, int width, int height)`
Callback appelé lors du redimensionnement de la fenêtre. Ajuste la zone de rendu OpenGL.

### `processInput(GLFWwindow* window)`
Traite les entrées utilisateur. Ferme la fenêtre si Échap est pressé.

### `main()`
Fonction principale qui :
- Initialise GLFW et crée la fenêtre
- Configure OpenGL avec GLAD
- Compile et lie les shaders
- Configure les buffers de sommets
- Exécute la boucle de rendu

## Erreurs gérées

Le programme vérifie et affiche les erreurs pour :
- Échec d'initialisation GLFW
- Échec de création de fenêtre
- Échec d'initialisation GLAD
- Erreurs de compilation des shaders
- Erreurs de liaison du programme shader

## Extensions possibles

- Ajout de textures
- Animation des sommets
- Gestion de la caméra
- Rendu 3D avec transformations
- Utilisation de modèles plus complexes

