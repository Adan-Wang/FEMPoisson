clear all; close all; clc;

coord = importdata("grid.csv");
T = importdata("triangles.csv");

figure;
triplot(T+1,coord(:,1), coord(:,2))