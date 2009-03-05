import os
import sys
import pygame

pygame.init()

picture = pygame.image.load("Img.jpg")
pygame.display.set_mode(picture.get_size())
surface = pygame.display.get_surface()

while True:
               
    os.system('./srv1')
    picture = pygame.image.load("Img.jpg")
    surface.blit(picture,(0,0))
    pygame.display.flip()
    
    key_pressed = pygame.key.get_pressed() 
    if key_pressed[pygame.K_ESCAPE]:
        sys.exit(0)
    