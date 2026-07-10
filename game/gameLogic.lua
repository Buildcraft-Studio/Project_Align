local timeClock = 0

local function incramentClock()
    timeClock = timeClock + 1
end
local statusCode = 0;
local function main()
while statusCode == 0 do
        if timeClock <= 25 then
        incramentClock()
        print(timeClock)
        elseif timeClock >= 25 then
            statusCode = 1
        end
    end
print("complete")
end


main()
