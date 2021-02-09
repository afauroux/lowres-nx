#!/bin/env python3

import hashlib 
import crcmod
from path import Path
from lxml import html
import sys
import requests
import os
import re

# Script to scrap the games and the infos '.dat' file in the format expected 
# by libretro
# Python version: 3.8
# Python non standard lib:
#   - pip install crcmod
#   - pip install lxml

baseurl = 'https://lowresnx.inutilis.com'
programs = lambda page: f"{baseurl}/programs.php?category=featured&page={page}"
topics = lambda topic: f"{baseurl}/{topic}"

def getpage(url, cache={}):
    if url in cache:
        return cache[url]
    else:
        cache[url] = requests.get(url)
        return cache[url]

i=0
links =  []

while True:
    page = getpage(programs(i))
    tree = html.fromstring(page.text)
    l = [t.values()[0] for t in tree.xpath('//div[@class="program"]/a[@href]')]
    if len(l)==0: break
    links.extend(l)
    i+=1
    

# Creates the directories
folder = 'Lowres-featured'
picfold = folder+'/pics'
os.makedirs(folder)
Pos.makedirs(picfold)

dat = f"""
clrmamepro (
	name "LowResNX"
	description "Program retro games in BASIC!"
	version "1.2"
	homepage "https://github.com/timoinutilis/lowres-nx/tree/retroarch"
)

"""

for link in links:
    p = getpage(topics(link))
    homepage = baseurl+'/'+link
    download = baseurl+'/'+game
    
    tree = html.fromstring(p.text)
    title =  tree.xpath('//h1')[0].text
    pic = tree.xpath('///img[@class="screenshot pixelated"]')[0].values()[1]
    game, name = tree.xpath('///div[@class="attachment-info"]//a')[0].values()
    author = tree.xpath('//p[@class="comment-info"]/span[@class="comment-user"]/a')[0].text 
    date =tree.xpath('///div[@class="attachment-info"]//div[@class="attachment-date"]')[0].text
    description = tree.xpath('/html/body/div[1]//p')[1]
    description = description.text_content().replace('\r','').split('\n')[0]
    name = re.findall('(.*)\..*$', name)[0]
    simplename = name.replace(" ", "_").replace("%","").replace(".","")

    # download pic
    p = getpage(baseurl+'/'+pic)
    with open(picfold+'/'+simplename+".png", "wb") as f:
        f.write(p.content)
        
    # download game
    p = getpage(baseurl+'/'+game)
    filename = folder+'/'+simplename+".nx"
    with open(filename, "wb") as f:
        f.write(p.content)
    
    # getting the md5
    md5 = hashlib.md5(p.content).hexdigest()
    size = len(p.content)

    # getting the crc
    f = crcmod.predefined.mkPredefinedCrcFun('crc32')
    crc = hex(f(p.content))[2:]
        
        
    # adding the Game info to the dat file
    dat += f"""
game (
    name "{name}"
    description "{description}"
    homepage "{homepage}"
    developer "{author}"
    download "{download}"
    rom ( name "{simplename+".nx"}" size "{size}" md5 "{md5}" crc "{crc}" )
)

    """

        
with open(folder+'/lowresnx.dat', 'w') as f:
    f.write(dat)