/****************************************************************************
 *
 * This is a part of HGCal offline software.
 * Authors:
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include <yaml-cpp/yaml.h>

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/SourceFactory.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/EventSetupRecordIntervalFinder.h"
#include "FWCore/Framework/interface/ESProducts.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/DataRecord/interface/HGCalLabTestConditionsRcd.h"
#include "CondFormats/HGCalObjects/interface/HGCalLabTestConditions.h"

class HGCalLabTestConditionsESSourceFromYAML : public edm::ESProducer, public edm::EventSetupRecordIntervalFinder {
public:
  explicit HGCalLabTestConditionsESSourceFromYAML(const edm::ParameterSet& iConfig)
      : filename_(iConfig.getParameter<std::string>("filename")) {
    setWhatProduced(this);
    findingRecord<HGCalLabTestConditionsRcd>();
  }

  std::unique_ptr<HGCalLabTestConditions> produce(const HGCalLabTestConditionsRcd&) { return parseYAML(filename_); }

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<std::string>("filename", {});
    descriptions.addWithDefaultLabel(desc);
  }

private:
  void setIntervalFor(const edm::eventsetup::EventSetupRecordKey&,
                      const edm::IOVSyncValue&,
                      edm::ValidityInterval& oValidity) override {
    oValidity = edm::ValidityInterval(edm::IOVSyncValue::beginOfTime(), edm::IOVSyncValue::endOfTime());
  }

  void parseNode(const std::string& node_name,
                 const YAML::Node& node,
                 std::unique_ptr<HGCalLabTestConditions>& cond) const {
    switch (node.Type()) {
      case YAML::NodeType::Null: {
        cond->addParameter(node_name, {});
      } break;
      case YAML::NodeType::Scalar: {
        try {
          cond->addParameter(node_name, node.as<int>());
        } catch (const YAML::BadConversion&) {
          return;
        }
      } break;
      case YAML::NodeType::Sequence:
        cond->addParameter(node_name, node.as<std::vector<int> >());
        break;
      case YAML::NodeType::Map: {
        for (const auto& subnode : node)
          parseNode(node_name + ":" + subnode.first.as<std::string>(), subnode.second, cond);
      } break;
      default:
        throw cms::Exception("HGCalLabTestConditionsESSourceFromYAML")
            << "Invalid node with key='" << node_name << "': unsupported type '" << node.Type() << "'.";
    }
  }

  std::unique_ptr<HGCalLabTestConditions> parseYAML(const std::string& filename) {
    auto cond = std::make_unique<HGCalLabTestConditions>();
    try {
      const auto yaml_file = YAML::LoadFile(filename);
      if (const auto config = yaml_file["metaData"]; config.IsDefined())
        for (const auto& params : config)
          parseNode(params.first.as<std::string>(), params.second, cond);
      else
        edm::LogWarning("HGCalLabTestConditionsESSourceFromYAML")
            << "The YAML configuration is missing a 'metaData' node. The conditions format may hence be invalid.";
    } catch (const YAML::BadFile& err) {
      throw cms::Exception("HGCalLabTestConditionsESSourceFromYAML") << "Bad file error: " << err.msg;
    } catch (const YAML::ParserException& err) {
      throw cms::Exception("HGCalLabTestConditionsESSourceFromYAML") << "Parser exception: " << err.msg;
    }
    return cond;
  }

  const std::string filename_;
};

DEFINE_FWK_EVENTSETUP_SOURCE(HGCalLabTestConditionsESSourceFromYAML);
