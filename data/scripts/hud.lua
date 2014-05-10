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
end
