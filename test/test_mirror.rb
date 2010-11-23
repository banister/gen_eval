direc = File.dirname(__FILE__)

require 'bacon'
require "#{direc}/../lib/gen_eval"

describe GenEval do
  it 'should make ivar changes to mirror reflect on original' do
    o = Object.new
    m = o.mirror
    o.instance_variable_defined?(:@x).should == false
    m.instance_variable_set(:@x, 10)
    o.instance_variable_get(:@x).should == 10
    m.unmirror
  end
end

