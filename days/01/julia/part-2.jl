
if abspath(PROGRAM_FILE) == @__FILE__
  if length(ARGS) < 1
    return
  end
  open(Base.ARGS[1]) do f
    lastnums = [0,0,0]
    linecount = 0
    result = 0
    for line in eachline(f)
      linecount += 1
      n = parse(Int, line)
      if (sum(lastnums[2:end]) + n > sum(lastnums)) & 
         (linecount > 3)
        result += 1
      end
      lastnums[1] = lastnums[2]
      lastnums[2] = lastnums[3]
      lastnums[3] = n
    end
    println(result)
  end
end

