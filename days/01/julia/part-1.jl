
if abspath(PROGRAM_FILE) == @__FILE__
  if length(ARGS) < 1
    return
  end
  open(Base.ARGS[1]) do f
    lastnum = 0
    result = 0
    for line in eachline(f)
      n = parse(Int, line)
      if n > lastnum
        result += 1
      end
      lastnum = n
    end
    println(result - 1)
  end
end

