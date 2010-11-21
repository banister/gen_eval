require '../lib/gen_eval'

$o = Object.new

class Hello
  class << self
    attr_reader :x
    def hello
      $o.gen_eval { @x = 2 }
    end
  end
end

Hello.hello
puts Hello.x



