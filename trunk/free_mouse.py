#!/usr/bin/env python
#free the mouse after an sdl crash

import pygame
pygame.init()
pygame.mouse.set_visible(False)
pygame.event.set_grab(True)
pygame.quit()
