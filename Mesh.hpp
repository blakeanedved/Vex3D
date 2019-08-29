//
//  Entity.cpp
//  Vex3D
//
//  Created by Blake Nedved on 8/27/19.
//  Copyright © 2019 Blake Nedved. All rights reserved.
//

#pragma once

#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "GameObject.hpp"
#include "Core.hpp"

namespace Vex {
    class MeshComponent {
    protected:
        enum BUFFERS {
            VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER
        };
        GLuint vbo[4];
        GLuint vao;
        
        unsigned int elementCount;
        
    public:
        MeshComponent(aiMesh *mesh);
        MeshComponent(std::vector<float> vertices, std::vector<unsigned int> indices);
        ~MeshComponent();
        
        auto Render() -> void;
    };
    
    class Mesh : public GameObject {
    protected:
        std::vector<std::unique_ptr<Vex::MeshComponent>> meshcomponents;
        
    public:
        Mesh(std::string name, glm::vec3 position, glm::vec3 rotation, const std::string& filename);
        Mesh(std::string name, glm::vec3 position, glm::vec3 rotation, std::vector<float> vertices, std::vector<unsigned int> indices);
        ~Mesh();
        auto internal_render() -> void;
    };
}

Vex::MeshComponent::MeshComponent(aiMesh *mesh){
    vbo[VERTEX_BUFFER] = 0;
    vbo[TEXCOORD_BUFFER] = 0;
    vbo[NORMAL_BUFFER] = 0;
    vbo[INDEX_BUFFER] = 0;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    elementCount = mesh->mNumFaces * 3;

    if(mesh->HasPositions()) {
        float *vertices = new float[mesh->mNumVertices * 3];
        for(int i = 0; i < mesh->mNumVertices; ++i) {
            vertices[i * 3] = mesh->mVertices[i].x;
            vertices[i * 3 + 1] = mesh->mVertices[i].y;
            vertices[i * 3 + 2] = mesh->mVertices[i].z;
        }

        glGenBuffers(1, &vbo[VERTEX_BUFFER]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        delete[] vertices;
    }


    if(mesh->HasTextureCoords(0)) {
        float *texCoords = new float[mesh->mNumVertices * 2];
        for(int i = 0; i < mesh->mNumVertices; ++i) {
            texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
            texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
        }

        glGenBuffers(1, &vbo[TEXCOORD_BUFFER]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        delete[] texCoords;
    }


    if(mesh->HasNormals()) {
        float *normals = new float[mesh->mNumVertices * 3];
        for(int i = 0; i < mesh->mNumVertices; ++i) {
            normals[i * 3] = mesh->mNormals[i].x;
            normals[i * 3 + 1] = mesh->mNormals[i].y;
            normals[i * 3 + 2] = mesh->mNormals[i].z;
        }

        glGenBuffers(1, &vbo[NORMAL_BUFFER]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(2);

        delete[] normals;
    }
    

    if(mesh->HasFaces()) {
        unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];
        for(int i = 0; i < mesh->mNumFaces; ++i) {
            indices[i * 3] = mesh->mFaces[i].mIndices[0];
            indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
            indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
        }

        glGenBuffers(1, &vbo[INDEX_BUFFER]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(3);

        delete[] indices;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Vex::MeshComponent::MeshComponent(std::vector<float> vertices, std::vector<unsigned int> indices){
    vbo[VERTEX_BUFFER] = 0;
    vbo[TEXCOORD_BUFFER] = 0;
    vbo[NORMAL_BUFFER] = 0;
    vbo[INDEX_BUFFER] = 0;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    elementCount = static_cast<int>(indices.size());
    
    // Vertices
    std::vector<float> data;
    for(int i = 0; i < (vertices.size() / 8); ++i) {
        data.push_back(vertices[i * 8]);
        data.push_back(vertices[i * 8 + 1]);
        data.push_back(vertices[i * 8 + 2]);
    }

    glGenBuffers(1, &vbo[VERTEX_BUFFER]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    data.clear();
    
    // Texture Co-Ordinates
    for(int i = 0; i < (vertices.size() / 8); ++i) {
        data.push_back(vertices[i * 8 + 3]);
        data.push_back(vertices[i * 8 + 4]);
    }

    glGenBuffers(1, &vbo[TEXCOORD_BUFFER]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data[0], GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    data.clear();
    
    // Normals
    for(int i = 0; i < (vertices.size() / 8); ++i) {
        data.push_back(vertices[i * 8 + 5]);
        data.push_back(vertices[i * 8 + 6]);
        data.push_back(vertices[i * 8 + 7]);
    }

    glGenBuffers(1, &vbo[NORMAL_BUFFER]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data[0], GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    data.clear();
    
    // Indices
    glGenBuffers(1, &vbo[INDEX_BUFFER]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);

    // Final Setup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Vex::MeshComponent::~MeshComponent(){
    if(vbo[VERTEX_BUFFER]) {
        glDeleteBuffers(1, &vbo[VERTEX_BUFFER]);
    }

    if(vbo[TEXCOORD_BUFFER]) {
        glDeleteBuffers(1, &vbo[TEXCOORD_BUFFER]);
    }

    if(vbo[NORMAL_BUFFER]) {
        glDeleteBuffers(1, &vbo[NORMAL_BUFFER]);
    }

    if(vbo[INDEX_BUFFER]) {
        glDeleteBuffers(1, &vbo[INDEX_BUFFER]);
    }

    glDeleteVertexArrays(1, &vao);
}

auto Vex::MeshComponent::Render() -> void {
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->elementCount, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

Vex::Mesh::Mesh(std::string name, glm::vec3 position, glm::vec3 rotation, const std::string& filename) : GameObject(name, position, rotation) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename, 0);
    if(!scene) {
        printf("Unable to load mesh: %s\n", importer.GetErrorString());
    }

    for(int i = 0; i < scene->mNumMeshes; ++i) {
        this->meshcomponents.push_back(std::make_unique<Vex::MeshComponent>(scene->mMeshes[i]));
    }
}

Vex::Mesh::Mesh(std::string name, glm::vec3 position, glm::vec3 rotation, std::vector<float> vertices, std::vector<unsigned int> indices) : GameObject(name, position, rotation) {
    this->meshcomponents.push_back(std::make_unique<Vex::MeshComponent>(vertices, indices));
}

Vex::Mesh::~Mesh(){
    this->meshcomponents.clear();
}

auto Vex::Mesh::internal_render() -> void {
    for (auto& mc : this->meshcomponents){
        mc->Render();
    }
}
