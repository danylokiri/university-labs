class Charscounter
  def self.charscounter(text)
    analyz = Hash.new(0)
    for i in text.chars
      analyz[i] += 1
    end
    analyz
  end
end