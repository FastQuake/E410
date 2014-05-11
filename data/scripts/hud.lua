require "vector"
HUD = {}
HUD.__index = HUD

function HUD.create(player)
	local h = {}
	setmetatable(h, HUD)
	h.player = player

	h.gui = {}
	--Create healthbar
	h.gui.hp = GUI.createBox()
	h.gui.hp:setScale(400,30)
	h.gui.hp:setPos(width/2-200,10)
	h.gui.hp:setColour(200,0,0,255)

	h.gui.ammo = GUI.createText()
	h.gui.ammo:setString(player.ammo)
	h.gui.ammo:setCharSize(32)
	h.gui.ammo:setPos(-100,-50)

	h.gui.cross = GUI.createImg()
	h.gui.cross:setImg("crosshairs.png")
	h.gui.cross:setZ(-2)

	h.gui.gun = GO.loadIQM("gun.iqm")
	return h
end

function HUD:show(bool)
	for k,v in pairs(self.gui) do
		v:setVisible(bool)
	end
end

function HUD:update()
	local scale = self.player.hp/100
	self.gui.hp:setPos(width/2 - (400*scale)/2,10)
	self.gui.hp:setScale(scale*400,30)
	self.gui.ammo:setString(player.ammo)
	
	if player.model ~= nil then
		--local x,y,z = player.model:getRot()
		local x,y,z = cam:getRot()
		local gunpos = Vector.create(player.model:getPos())
		local fwd = Vector.create(cam:getLookat())
		fwd.y =0
		fwd = fwd:normalize()
		local right = Vector.cross(fwd,Vector.create(0,1,0))
		gunpos.y = gunpos.y+1.5
		gunpos = gunpos +Vector.scalarMul(0.3,right)
		h.gui.gun:setPos(gunpos:get())
		h.gui.gun:setRot(-x,y,z)
		h.gui.cross:setPos(width/2-16,height/2-16)
	end
end
