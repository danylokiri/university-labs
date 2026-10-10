require "charscounter"
require "rspec"

RSpec.describe Charscounter do
  it "рахує символи" do
    result = Charscounter.charscounter("asjhdgajdyryryryrnsdjcvbvhsdgtfv][ad")
    puts result
    expect(result).to eq({"["=>1, "]"=>1, "a"=>3, "b"=>1, "c"=>1, "d"=>5, "f"=>1, "g"=>2, "h"=>2, "j"=>3, "n"=>1, "r"=>4, "s"=>3, "t"=>1, "v"=>3, "y"=>4})
  end
end