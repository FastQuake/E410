require "vector"
Bullet = {}
Bullet.__index = Bullet

function Bullet.create(pos,stop)
	b = {}
	setmetatable(b, Bullet)

	b.pos = pos
	b.spos = pos
	b.stop = stop
	b.dir = (stop-pos):normalize()
	b.sprite = GO.createSprite("glow.png")
	b.sprite:setPos(pos:get())

	return b
end
