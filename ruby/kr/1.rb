arr = [1,2,3,4,5]
arr.include?(3)         # поверне true
for i in arr
  if arr[i] == 5
    true          # поверне true
    break
  end
end

hash = {a: 1, b: 2, c: 3, d: 4, e: 5}
hash.value?(4)         # поверне true
for key, value in hash
  if value == 5
    true      # поверне true
    break
  end
end

arr[8].nil?        #поверне true, бо наш масив має тільки 5 елементів