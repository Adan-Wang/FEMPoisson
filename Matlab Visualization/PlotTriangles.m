clear all; close all; clc;

coord = importdata("grid.csv");
T = importdata("triangles.csv");

figure;
triplot(T,coord(:,1), coord(:,2))