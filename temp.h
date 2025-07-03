#pragma once

#include <vector>
#include "Mesh.h"
#include "geometry/AABB.h"
#include "Transform.h"

class Entity {
public:
    std::vector<Mesh> parts;    ///< 서브 메시들
    Material material;          ///< 엔티티 공통 머티리얼
    Transform transform;        ///< 로컬 변환 정보

    /// 엔티티 로컬 좌표계에서 AABB를 다시 계산
    void ComputeLocalAABB() {
        // 초기화: empty AABB
        localAABB = Geometry::AABB();
        // 각 파트 메시의 로컬 AABB를 가져와 합집합
        for (auto& part : parts) {
            // 메시 자체의 로컬 AABB 갱신
            part.ComputeLocalAABB();
            // 파트 메시의 AABB와 합집합
            localAABB = Geometry::AABB::Union(localAABB, part.GetLocalAABB());
        }
        dirtyAABB = false;
    }

    /// 로컬 AABB를 리턴 (캐싱 및 dirty-flag 적용)
    const Geometry::AABB& GetLocalAABB() {
        if (dirtyAABB) {
            ComputeLocalAABB();
        }
        return localAABB;
    }

    /// 예: Transform 변경 시 dirty 플래그 설정
    void SetTransform(const Transform& t) {
        transform = t;
        // 로컬 AABB는 transform 영향을 받지 않으므로 여기선 건드리지 않음
    }

    /// 메시 파트가 변경될 때 호출
    void MarkDirty() {
        dirtyAABB = true;
    }

    void Update(float delta) {
        // 로직 수행 중 파트 변경 시 MarkDirty()를 호출하면,
        // 다음 GetLocalAABB()에서 재계산됨
    }

private:
    Geometry::AABB localAABB;  ///< 캐시된 로컬 AABB
    bool dirtyAABB = true;     ///< 재계산 필요 플래그
};
