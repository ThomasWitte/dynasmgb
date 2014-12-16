#!/usr/bin/env lua

infile = arg[1]
outfile = arg[2]

print('infile: ' .. infile)
print('outfile: ' .. outfile)

outfile = io.open(outfile, 'w+')

in_interrupt = 0
stack = 0

last_cmd = {}
rep_count = 1

for line in io.lines(infile) do
    for op, a1, a2 in line:gmatch('(%w*) from (%d*) to (%d*)') do
        if op ~= 'interrupt' then
            if last_cmd.op == op and last_cmd.a1 == a1 and last_cmd.a2 == a2 then
                rep_count = rep_count+1
            else
                if(rep_count > 1) then
                    outfile:write(' (x', rep_count, ')')
                end
                rep_count = 1
                last_cmd = {op = op, a1 = a1, a2 = a2}
                outfile:write('\n')
                if in_interrupt > 0 then
                    outfile:write('>')
                end
                for i=1, stack, 1 do
                    outfile:write('\t')
                end
                outfile:write(op, ' ', a1, '->', a2)
            end
        end

        if op == 'interrupt' then
            if in_interrupt > 0 then
                print('nested interrupt!')
            end
            in_interrupt = in_interrupt+1
        elseif op == 'iret' then
            if in_interrupt <= 0 then
                print('iret without interrupt!')
            end
            in_interrupt = in_interrupt-1
        elseif op == 'call' and tonumber(a2) > 64 then
            stack = stack+1
        elseif op == 'ret' then
            stack = stack-1
        end
    end
end

print('stack: ', stack)

outfile:close()
